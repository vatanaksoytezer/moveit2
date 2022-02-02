# Return a list of nodes we commonly launch for the demo. Nice for testing use.
import launch
import os
import xacro
import yaml

from ament_index_python.packages import get_package_share_directory
from launch.actions import ExecuteProcess
from launch_ros.actions import Node
from launch_ros.descriptions import ComposableNode
from launch_ros.actions import ComposableNodeContainer


def load_file(package_name, file_path):
    package_path = get_package_share_directory(package_name)
    absolute_file_path = os.path.join(package_path, file_path)

    try:
        with open(absolute_file_path, "r") as file:
            return file.read()
    except EnvironmentError:  # parent of IOError, OSError *and* WindowsError where available
        return None


def load_yaml(package_name, file_path):
    package_path = get_package_share_directory(package_name)
    absolute_file_path = os.path.join(package_path, file_path)

    try:
        with open(absolute_file_path, "r") as file:
            return yaml.safe_load(file)
    except EnvironmentError:  # parent of IOError, OSError *and* WindowsError where available
        return None


def get_robot_description():
    robot_description_config = xacro.process_file(
        os.path.join(
            get_package_share_directory("moveit_resources_panda_moveit_config"),
            "config",
            "panda.urdf.xacro",
        )
    )
    robot_description = {"robot_description": robot_description_config.toxml()}
    return robot_description


def get_robot_description_semantic():
    robot_description_semantic_config = load_file(
        "moveit_resources_panda_moveit_config", "config/panda.srdf"
    )
    robot_description_semantic = {
        "robot_description_semantic": robot_description_semantic_config
    }
    return robot_description_semantic


def generate_common_hybrid_launch_description():
    robot_description = get_robot_description()

    robot_description_semantic = get_robot_description_semantic()

    kinematics_yaml = load_yaml(
        "moveit_resources_panda_moveit_config", "config/kinematics.yaml"
    )

    # The global planner uses the typical OMPL parameters
    planning_pipelines_config = {
        "ompl": {
            "planning_plugin": "ompl_interface/OMPLPlanner",
            "request_adapters": """default_planner_request_adapters/AddTimeOptimalParameterization default_planner_request_adapters/FixWorkspaceBounds default_planner_request_adapters/FixStartStateBounds default_planner_request_adapters/FixStartStateCollision default_planner_request_adapters/FixStartStatePathConstraints""",
            "start_state_max_bounds_error": 0.1,
        }
    }
    ompl_planning_yaml = load_yaml(
        "moveit_resources_panda_moveit_config", "config/ompl_planning.yaml"
    )
    planning_pipelines_config["ompl"].update(ompl_planning_yaml)

    moveit_simple_controllers_yaml = load_yaml(
        "moveit_resources_panda_moveit_config", "config/panda_controllers.yaml"
    )
    moveit_controllers = {
        "moveit_simple_controller_manager": moveit_simple_controllers_yaml,
        "moveit_controller_manager": "moveit_simple_controller_manager/MoveItSimpleControllerManager",
    }

    # Any parameters that are unique to your plugins go here
    common_hybrid_planning_param = load_yaml(
        "moveit_hybrid_planning", "config/common_hybrid_planning_params.yaml"
    )
    global_planner_param = load_yaml(
        "moveit_hybrid_planning", "config/global_planner.yaml"
    )
    local_planner_param = load_yaml(
        "moveit_hybrid_planning", "config/local_planner.yaml"
    )
    hybrid_planning_manager_param = load_yaml(
        "moveit_hybrid_planning", "config/hybrid_planning_manager.yaml"
    )

    # Generate launch description with multiple components
    container = ComposableNodeContainer(
        name="hybrid_planning_container",
        namespace="/",
        package="rclcpp_components",
        executable="component_container_mt",
        composable_node_descriptions=[
            ComposableNode(
                package="moveit_hybrid_planning",
                plugin="moveit::hybrid_planning::GlobalPlannerComponent",
                name="global_planner",
                parameters=[
                    common_hybrid_planning_param,
                    global_planner_param,
                    robot_description,
                    robot_description_semantic,
                    kinematics_yaml,
                    planning_pipelines_config,
                    moveit_controllers,
                ],
            ),
            ComposableNode(
                package="moveit_hybrid_planning",
                plugin="moveit::hybrid_planning::LocalPlannerComponent",
                name="local_planner",
                parameters=[
                    common_hybrid_planning_param,
                    local_planner_param,
                    robot_description,
                    robot_description_semantic,
                    kinematics_yaml,
                ],
            ),
            ComposableNode(
                package="moveit_hybrid_planning",
                plugin="moveit::hybrid_planning::HybridPlanningManager",
                name="hybrid_planning_manager",
                parameters=[
                    common_hybrid_planning_param,
                    hybrid_planning_manager_param,
                ],
            ),
            ComposableNode(
                package="tf2_ros",
                executable="static_transform_publisher",
                name="static_transform_publisher",
                output="log",
                arguments=[
                    "0.0",
                    "0.0",
                    "0.0",
                    "0.0",
                    "0.0",
                    "0.0",
                    "world",
                    "panda_link0",
                ],
            ),
            ComposableNode(
                package="robot_state_publisher",
                executable="robot_state_publisher",
                name="robot_state_publisher",
                output="both",
                parameters=[robot_description],
            ),
        ],
        output="screen",
    )

    # RViz
    rviz_config_file = (
        get_package_share_directory("moveit_hybrid_planning")
        + "/config/hybrid_planning_demo.rviz"
    )
    rviz_node = Node(
        package="rviz2",
        executable="rviz2",
        name="rviz2",
        output="log",
        arguments=["-d", rviz_config_file],
        parameters=[robot_description, robot_description_semantic],
    )

    # ros2_control using FakeSystem as hardware
    ros2_controllers_path = os.path.join(
        get_package_share_directory("moveit_hybrid_planning"),
        "config",
        "demo_controller.yaml",
    )
    ros2_control_node = Node(
        package="controller_manager",
        executable="ros2_control_node",
        parameters=[robot_description, ros2_controllers_path],
        output={
            "stdout": "screen",
            "stderr": "screen",
        },
    )

    # Load controllers
    load_controllers = []
    for controller in [
        "joint_state_broadcaster",
        "panda_joint_group_position_controller",
    ]:
        load_controllers += [
            ExecuteProcess(
                cmd=["ros2 run controller_manager spawner {}".format(controller)],
                shell=True,
                output="screen",
            )
        ]

    launched_nodes = [
        container,
        # rviz_node,
        ros2_control_node,
    ] + load_controllers

    return launched_nodes
