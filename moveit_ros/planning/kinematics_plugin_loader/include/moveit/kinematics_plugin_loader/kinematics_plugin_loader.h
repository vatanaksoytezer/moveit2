/*********************************************************************
 * Software License Agreement (BSD License)
 *
 *  Copyright (c) 2012, Willow Garage, Inc.
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above
 *     copyright notice, this list of conditions and the following
 *     disclaimer in the documentation and/or other materials provided
 *     with the distribution.
 *   * Neither the name of Willow Garage nor the names of its
 *     contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 *  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 *  COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 *  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 *  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 *  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 *  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 *  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 *********************************************************************/

/* Author: Ioan Sucan, Dave Coleman */

#pragma once

#include <moveit/macros/class_forward.h>
#include <moveit/robot_model/robot_model.h>
#include <moveit/kinematics_base/kinematics_base.h>
#include <moveit_node_interface/moveit_node_interface.hpp>

namespace kinematics_plugin_loader
{
MOVEIT_CLASS_FORWARD(KinematicsPluginLoader);  // Defines KinematicsPluginLoaderPtr, ConstPtr, WeakPtr... etc

/** \brief Helper class for loading kinematics solvers */
class KinematicsPluginLoader
{
public:
  /** \brief Load the kinematics solvers based on information on the
      ROS parameter server. Take node as an argument and as optional argument the name of the
      ROS parameter under which the robot description can be
      found. This is passed to the kinematics solver initialization as
      well as used to read the SRDF document when needed. */
  KinematicsPluginLoader(const rclcpp::Node::SharedPtr& node,
                         const std::string& robot_description = "robot_description",
                         double default_search_resolution = 0.0)
    : node_interface_(
          std::make_shared<moveit::node_interface::NodeInterface>(moveit::node_interface::NodeInterface(node)))
    , robot_description_(robot_description)
    , default_search_resolution_(default_search_resolution)
  {
  }

  KinematicsPluginLoader(moveit::node_interface::NodeInterfaceSharedPtr& node_interface,
                         const std::string& robot_description = "robot_description",
                         double default_search_resolution = 0.0)
    : node_interface_(node_interface)
    , robot_description_(robot_description)
    , default_search_resolution_(default_search_resolution)
  {
  }

  /** \brief Use a default kinematics solver (\e solver_plugin) for
      all the groups in the robot model. The default timeout for the
      solver is \e solve_timeout and the default number of IK attempts
      is \e ik_attempts. Take node as an argument and as optional argument the name of the ROS
      parameter under which the robot description can be found. This
      is passed to the kinematics solver initialization as well as
      used to read the SRDF document when needed. */
  KinematicsPluginLoader(moveit::node_interface::NodeInterfaceSharedPtr& node_interface,
                         const std::string& solver_plugin, double solve_timeout,
                         const std::string& robot_description = "robot_description",
                         double default_search_resolution = 0.0)
    : node_interface_(node_interface)
    , robot_description_(robot_description)
    , default_search_resolution_(default_search_resolution)
    , default_solver_plugin_(solver_plugin)
    , default_solver_timeout_(solve_timeout)
  {
  }

  KinematicsPluginLoader(const rclcpp::Node::SharedPtr& node, const std::string& solver_plugin, double solve_timeout,
                         const std::string& robot_description = "robot_description",
                         double default_search_resolution = 0.0)
    : node_interface_(
          std::make_shared<moveit::node_interface::NodeInterface>(moveit::node_interface::NodeInterface(node)))
    , robot_description_(robot_description)
    , default_search_resolution_(default_search_resolution)
    , default_solver_plugin_(solver_plugin)
    , default_solver_timeout_(solve_timeout)
  {
  }

  /** \brief Get a function pointer that allocates and initializes a kinematics solver. If not previously called, this
   * function reads the SRDF and calls the variant below. */
  moveit::core::SolverAllocatorFn getLoaderFunction();

  /** \brief Get a function pointer that allocates and initializes a kinematics solver. If not previously called, this
   * function reads ROS parameters for the groups defined in the SRDF. */
  moveit::core::SolverAllocatorFn getLoaderFunction(const srdf::ModelSharedPtr& srdf_model);

  /** \brief Get the groups for which the function pointer returned by getLoaderFunction() can allocate a solver */
  const std::vector<std::string>& getKnownGroups() const
  {
    return groups_;
  }

  /** \brief Get a map from group name to default IK timeout */
  const std::map<std::string, double>& getIKTimeout() const
  {
    return ik_timeout_;
  }

  void status() const;

private:
  moveit::node_interface::NodeInterfaceSharedPtr node_interface_;
  std::string robot_description_;
  double default_search_resolution_;

  MOVEIT_CLASS_FORWARD(KinematicsLoaderImpl);  // Defines KinematicsLoaderImplPtr, ConstPtr, WeakPtr... etc
  KinematicsLoaderImplPtr loader_;

  std::vector<std::string> groups_;
  std::map<std::string, double> ik_timeout_;

  // default configuration
  std::string default_solver_plugin_;
  double default_solver_timeout_;
};
}  // namespace kinematics_plugin_loader
