^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Changelog for package moveit_ros_occupancy_map_monitor
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Forthcoming
-----------
* Fix boost linking errors for Windows (`#957 <https://github.com/vatanaksoytezer/moveit2/issues/957>`_)
* Replace NULL with nullptr (`#961 <https://github.com/vatanaksoytezer/moveit2/issues/961>`_)
  * Fixes `#841 <https://github.com/vatanaksoytezer/moveit2/issues/841>`_
* moveit_build_options()
  Declare common build options like CMAKE_CXX_STANDARD, CMAKE_BUILD_TYPE,
  and compiler options (namely warning flags) once.
  Each package depending on moveit_core can use these via moveit_build_options().
* Contributors: Akash, Robert Haschke, Stephanie Eng

2.3.2 (2021-12-29)
------------------

2.3.1 (2021-12-23)
------------------
* Replaced C-Style Cast with C++ Style Cast. (`#935 <https://github.com/ros-planning/moveit2/issues/935>`_)
* Add codespell to precommit, fix A LOT of spelling mistakes (`#934 <https://github.com/ros-planning/moveit2/issues/934>`_)
* Get rid of "std::endl" (`#918 <https://github.com/ros-planning/moveit2/issues/918>`_)
* Fix boost linking errors (`#900 <https://github.com/ros-planning/moveit2/issues/900>`_)
* Enforce package.xml format 3 Schema (`#779 <https://github.com/ros-planning/moveit2/issues/779>`_)
* Update Maintainers of MoveIt package (`#697 <https://github.com/ros-planning/moveit2/issues/697>`_)
* Prefer std::make_shared over new operator (`#2756 <https://github.com/ros-planning/moveit/issues/2756>`_)
* Contributors: Dave Coleman, David V. Lu!!, Henning Kayser, Kaustubh, Michael Görner, Parthasarathy Bana, Robert Haschke, Vatan Aksoy Tezer

2.3.0 (2021-10-08)
------------------
* Error if Octomap 'map_frame' is not provided (`#667 <https://github.com/ros-planning/moveit2/issues/667>`_)
* OccupancyMapMonitor tests using Dependency Injection (`#569 <https://github.com/ros-planning/moveit2/issues/569>`_)
* Refactors for OccMapTree in PlanningScene (`#2684 <https://github.com/ros-planning/moveit2/issues/2684>`_)
* Move OccMapTree to moveit_core/collision_detection
* Contributors: AndyZe, Henning Kayser, Simon Schmeisser, Tyler Weaver, Jafar Abdi

2.2.1 (2021-07-12)
------------------

2.2.0 (2021-06-30)
------------------
* [sync] with MoveIt's master branch up-to https://github.com/ros-planning/moveit/commit/0d0a6a171b3fbea97a0c4f284e13433ba66a4ea4
  * Add missing OCTOMAP_INCLUDE_DIRS (`#2671 <https://github.com/ros-planning/moveit/issues/2671>`_)
  * document solution in ROS_ERROR on failed self-filtering (`#2627 <https://github.com/ros-planning/moveit/issues/2627>`_)
  * It's not an error not to define a plugin (`#2521 <https://github.com/ros-planning/moveit/issues/2521>`_)
* Contributors: 0Nel, JafarAbdi, Michael Görner, Robert Haschke, Simon Schmeisser, Tyler Weaver, v4hn

2.1.4 (2021-05-31)
------------------

2.1.3 (2021-05-22)
------------------

2.1.2 (2021-04-20)
------------------

2.1.1 (2021-04-12)
------------------
* Fix EXPORT install in CMake (`#372 <https://github.com/ros-planning/moveit2/issues/372>`_)
* Porting moveit_ros_perception/pointcloud_octomap_updater (`#307 <https://github.com/ros-planning/moveit2/issues/307>`_)
* Fix repo URLs in package.xml files
* Contributors: Henning Kayser, Tyler Weaver, Yu Yan

2.1.0 (2020-11-23)
------------------
* [maint] Wrap common cmake code in 'moveit_package()' macro (`#285 <https://github.com/ros-planning/moveit2/issues/285>`_)
  * New moveit_package() macro for compile flags, Windows support etc
  * Add package 'moveit_common' as build dependency for moveit_package()
  * Added -Wno-overloaded-virtual compiler flag for moveit_ros_planners_ompl
* Contributors: Lior Lustgarten

2.0.0 (2020-02-17)
------------------
* [port] Port occupancy_map_monitor to ROS2 (`#148 <https://github.com/ros-planning/moveit2/issues/148>`_)
* [fix] replace rosunit -> ament_cmake_gtest
* [port] Port rdf_loader to ROS2 (`#104 <https://github.com/ros-planning/moveit2/issues/104>`_)
* Contributors: Henning Kayser, Jafar Abdi, Yu Yan

1.1.1 (2020-10-13)
------------------
* [fix] clang-tidy issues on Travis (`#2337 <https://github.com/ros-planning/moveit/issues/2337>`_)
  * Replace typedefs with using declarations
  * Move default destructor definitions to headers
  * Silent spurious clang-tidy warning
  * Move variable definitions to their usage location
* [maint] Cleanup MSA includes (`#2351 <https://github.com/ros-planning/moveit/issues/2351>`_)
* [maint] Add comment to MOVEIT_CLASS_FORWARD (`#2315 <https://github.com/ros-planning/moveit/issues/2315>`_)
* Contributors: Felix von Drigalski, Robert Haschke

1.1.0 (2020-09-04)
------------------

1.0.6 (2020-08-19)
------------------
* [maint] Migrate to clang-format-10
* Contributors: Robert Haschke

1.0.5 (2020-07-08)
------------------

1.0.4 (2020-05-30)
------------------

1.0.3 (2020-04-26)
------------------
* [fix]   Add error message on failure to initialize occupancy map monitor (`#1873 <https://github.com/ros-planning/moveit/issues/1873>`_)
* [fix]   Update occupancy grid when loaded from file (`#1594 <https://github.com/ros-planning/moveit/issues/1594>`_)
* [maint] Apply clang-tidy fix (`#1394 <https://github.com/ros-planning/moveit/issues/1394>`_)
* [maint] Windows build fixes
  * Fix header inclusion and other MSVC build errors (`#1636 <https://github.com/ros-planning/moveit/issues/1636>`_)
  * Fix binary artifact install locations. (`#1575 <https://github.com/ros-planning/moveit/issues/1575>`_)
* [maint] Use CMAKE_CXX_STANDARD to enforce c++14 for portability (`#1607 <https://github.com/ros-planning/moveit/issues/1607>`_)
* [maint] move occupancy_map_monitor into its own package (`#1533 <https://github.com/ros-planning/moveit/issues/1533>`_)
* Contributors: Bjar Ne, Dale Koenig, Raphael Druon, Robert Haschke, Sean Yen, Simon Schmeisser, Yu, Yan, jschleicher
