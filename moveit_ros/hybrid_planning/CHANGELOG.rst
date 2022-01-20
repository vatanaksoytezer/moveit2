^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Changelog for package moveit_hybrid_planning
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

2.3.3 (2022-01-20)
------------------

2.3.2 (2021-12-29)
------------------
* Fix syntax (`#939 <https://github.com/ros-planning/moveit2/issues/939>`_)
  Co-authored-by: AndyZe <zelenak@picknik.ai>
* Contributors: Sebastian Jahr

2.3.1 (2021-12-23)
------------------
* Bump new packages to 2.3.0
* Add codespell to precommit, fix A LOT of spelling mistakes (`#934 <https://github.com/ros-planning/moveit2/issues/934>`_)
* CMake fix and cleanup abstract class (`#6 <https://github.com/ros-planning/moveit2/issues/6>`_)
  * Install hybrid_planning_demo_node separately to avoid exporting it
  * Fix exported include directory
  * Remove getTargetWayPointIndex() from abstract trajectory operator class
  * Delete unused getTargetWayPointIndex()
* Standardize with moveit_cpp parameters. Fix some parameter loading errors
* Enable reaction to planner failure in the planner logic (`#3 <https://github.com/ros-planning/moveit2/issues/3>`_)
  * Add unsuccessful action Hybrid Planning events and handle them in logic
  * Replace std::once with simple bool variable
  * Remove unneeded variable and update comments
  Don't const& for built-in types
  Co-authored-by: Tyler Weaver <tylerjw@gmail.com>
* Update robot state properly
  Update robot state properly; remove debug prints; clang format
  Check if the local planner is stuck within the forward_traj plugin, not local_planner_component
* Pass GlobalPlanner failing to HybridPlanningManager
* Move common launch elements to a Python file, for easy re-use
  Refactor Global and Local Planner Components into NodeClasses
  Add a simple launch test (`#1 <https://github.com/ros-planning/moveit2/issues/1>`_)
  Try to fix plugin export; add helpful debug when stuck
  Error if global planning fails
  READY and AWAIT_TRAJ states are redundant
  Lock the planning scene as briefly as possible
  Specify joint group when checking waypoint distance
  Implement a reset() function in the local planner
  Detect when the local planner gets stuck
* Add generic global planner plugin, support MotionSequenceRequest (`#585 <https://github.com/ros-planning/moveit2/issues/585>`_)
  Fix hybrid planning include folders (`#675 <https://github.com/ros-planning/moveit2/issues/675>`_)
  Order stuff in the CMakeLists.txt and remove control_box package
  Update README
  Move member initialization to initializer lists
  Remove control_box include dependency
  Replace "loop-run" with "iteration"
  Remove cpp interface class constructors and destructors
  Use joint_state_broadcaster, clean up test node, add execution dependencies
  Use only plugin interface header files and add missing dependencies
  Clean up constructor and destructor definitions
  Declare missing parameter in moveit_planning_pipeline_plugin
  Move rclcpp::Loggers into anonymous namespaces
  Switch CI branches to feature/hybrid_planning
  Update message name
  Remove moveit_msgs from .repos file
  Update github workflows
  Remove note from readme about building from source
  Minor renamings, make reset() noexcept
  Check for vector length of zero
  Load moveit_cpp options with the Options helper. Reduces LOC.
  Set the planning parameters within plan()
  Function renaming
  Authors and descriptions in header files only. New header file for error code interface.
  Update namespacing
  Use default QoS for subscribers
  Better progress comparison
  Add publish_joint_positions and publish_joint_velocities param
  Grammar and other minor nitpicks
  Restore moveit_msgs to .repos, for now
* Refactor local planner plugins (`#447 <https://github.com/ros-planning/moveit2/issues/447>`_)
  * Add reset method for trajectory operator and local constraint sampler
  * Refactor next_waypoint_sampler into simple_sampler
  * Include collision checking to forward_trajectory and remove unneeded plugin
  * Fix formatting and plugin description
  * Update README and add missing planner logic plugin description
  Add TODO to use lifecycle components nodes to trigger initialization
  Return a reaction result instead of bool on react()
  Set invalidation flag to false on reset() in ForwardTrajectory local solver
  Return local planner feedback from trajectory operator function calls
  Fix segfault caused by passing through the global trajectory
  Update comment, unify logging and add missing config parameters
  Update to rolling
* Restructure hybrid_planning package (`#426 <https://github.com/ros-planning/moveit2/issues/426>`_)
  * Add forward_trajectory local solver plugin (`#359 <https://github.com/ros-planning/moveit2/issues/359>`_)
  * Use ros2_control based joint simulation and remove unnecessary comment
  * Update copyrights
  * Restructure hybrid planning package
  * Fix formatting and add missing time stamp in local solver plugin
  * Remove unnecessary logging and param loading
  * Enable different interfaces between local planner and controller
  * Use JointGroupPositionController as demo hardware controller
* Code cleanup & MoveIt best practices (`#351 <https://github.com/ros-planning/moveit2/issues/351>`_)
  * Export missing plugins
  * Use std::chrono_literals
  * Construct smart pointers with std::make\_* instead of 'new'
  * Fixup const-ref in function signatures, reduce copies
  * Improve planning scene locking, robot state processing, controller logic
* Refine local planner component (`#326 <https://github.com/ros-planning/moveit2/issues/326>`_)
  * Make local planner component generic
  * Add next_waypoint_sampler trajectory operator
  * Update hybrid planning test to include collision object
  * Clean up code and fix minor bugs.
  * Update local planner component parameter
  * Add local collision constraint solver
  * Update planning scene topic name and test node
  * Fix bugs and runtime errors in local planner component and it's plugins
  * Add collision object that invalidates global trajectory
  * Add simple "stop in front of collision object" demo
  * Add hybrid planning manager reaction to local planner feedback
  * Fix ament_lint_cmake
  * Ensure that local planner start command and collision event are send once
  * Add simple replanning logic plugin
  * Use current state as start state for global planner
  * Use RobotTrajectory instead of constraint vector describe local problem
  * Add PlanningSceneMonitorPtr to local solver plugin
  * Add local planner frequency parameter
  * Use PID controller to create control outputs for robot controller
  * Add hybrid_planning_manager config file
  * Add more complex test node
  * Update README
  * Reset index in next_waypoint_sampler
  * Use correct isPathValid() interface
  * Rename path_invalidation flag
  * Read planning scene instead of cloning it in local planner
  * Add TODO creator
  * Rename local constraint solver plugin
  * Use read-locked access to the planning scene for collision checking
  * Rename constraint_solver into local_constraint_solver
  * Add missing pointer initialization
* Hybrid planning architecture (`#311 <https://github.com/ros-planning/moveit2/issues/311>`_)
  * Add hybrid_planning architecture skeleton code
  * Add simple state machines to hybrid planning manager and local planner
  * Initial global planner prototype implementation
  * Forward joint_trajectory with local planner
  * Forward hybrid planning motion request to global planner
  * Abstract planner logic from hybrid planning manager by using a plugin
  * Implement single plan execution logic
  * Add test launch files, RViz and demo config
  * Add test for motion_planning_request
* Contributors: AndyZe, David V. Lu!!, Henning Kayser, Jens Vanhooydonck, Sebastian Jahr
