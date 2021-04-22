import os
import yaml
import unittest

from launch import LaunchDescription
from launch_ros.actions import Node
import launch_testing
from ament_index_python.packages import get_package_share_directory
from launch_testing.util import KeepAliveProc

import pytest


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
            return yaml.full_load(file)
    except EnvironmentError:  # parent of IOError, OSError *and* WindowsError where available
        return None


@pytest.mark.rostest
def generate_test_description():

    # Component yaml files are grouped in separate namespaces
    robot_description_config = load_file(
        "moveit_resources_panda_description", "urdf/panda.urdf"
    )
    robot_description = {"robot_description": robot_description_config}

    robot_description_semantic_config = load_file(
        "moveit_resources_panda_moveit_config", "config/panda.srdf"
    )
    robot_description_semantic = {
        "robot_description_semantic": robot_description_semantic_config
    }
    kinematics_yaml = load_yaml(
        "moveit_resources_panda_moveit_config", "config/kinematics.yaml"
    )
    robot_description_kinematics = {"robot_description_kinematics": kinematics_yaml}
    test_param = load_yaml("moveit_kinematics", "config/panda-kdl-test.yaml")

    private_params = {
        "seed": [-0.5, -0.5, 0.3, -2, 0.8, 1.8, 1.9],
        "consistency_limits": [0.4, 0.4, 0.4, 0.4, 0.4, 0.4, 0.4],
        "num_fk_tests": 100,
        "num_ik_tests": 100,
    }
    panda_kdl = Node(
        package="moveit_kinematics",
        executable="test_kinematics_plugin",
        name="panda_kdl",
        parameters=[
            robot_description,
            robot_description_semantic,
            robot_description_kinematics,
            test_param,
            private_params,
        ],
        output="screen",
    )

    return LaunchDescription(
        [
            panda_kdl,
            KeepAliveProc(),
            launch_testing.actions.ReadyToTest(),
        ]
    ), {"panda_kdl": panda_kdl},

class TestTerminatingProcessStops(unittest.TestCase):
    def test_gtest_run_complete(self, proc_info, panda_kdl):
        proc_info.assertWaitForShutdown(process=panda_kdl, timeout=4000.0)

@launch_testing.post_shutdown_test()
class TestOutcome(unittest.TestCase):
    def test_exit_codes(self, proc_info):
        launch_testing.asserts.assertExitCodes(proc_info)
