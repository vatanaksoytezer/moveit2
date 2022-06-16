/*********************************************************************
 * Software License Agreement (BSD License)
 *
 *  Copyright (c) 2021, PickNik Robotics
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
 *   * Neither the name of PickNik Robotics nor the names of its
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

/* Author: David V. Lu!! */

#include <moveit/rdf_loader/synchronized_string_parameter.h>

namespace rdf_loader
{
static const rclcpp::Logger LOGGER = rclcpp::get_logger("moveit_ros.robot_model_loader");

std::string SynchronizedStringParameter::loadInitialValue(const node_interface::NodeInterfaceSharedPtr& node_interface,
                                                          const std::string& name, StringCallback parent_callback,
                                                          bool default_continuous_value, double default_timeout)
{
  nodeInterface_ = node_interface;
  name_ = name;
  parent_callback_ = parent_callback;

  if (getMainParameter())
  {
    if (shouldPublish())
    {
      // Transient local is similar to latching in ROS 1.
      string_publisher_ = rclcpp::create_publisher<std_msgs::msg::String>(nodeInterface_->parameter_interface_,
                                                                          nodeInterface_->topic_interface_, name_,
                                                                          rclcpp::QoS(1).transient_local());

      std_msgs::msg::String msg;
      msg.data = content_;
      string_publisher_->publish(msg);
    }
    return content_;
  }

  // Load topic parameters
  std::string keep_open_param = name_ + "_continuous";
  if (!nodeInterface_->parameter_interface_->has_parameter(keep_open_param))
  {
    nodeInterface_->parameter_interface_->declare_parameter(keep_open_param,
                                                            rclcpp::ParameterValue(default_continuous_value));
  }
  rclcpp::Parameter keep_open_parameter;
  nodeInterface_->parameter_interface_->get_parameter(keep_open_param, keep_open_parameter);
  bool keep_open = keep_open_parameter.as_bool();

  std::string timeout_param = name_ + "_timeout";
  if (!nodeInterface_->parameter_interface_->has_parameter(timeout_param))
  {
    nodeInterface_->parameter_interface_->declare_parameter(timeout_param, rclcpp::ParameterValue(default_timeout));
  }
  rclcpp::Parameter d_timeout_parameter;
  nodeInterface_->parameter_interface_->get_parameter(timeout_param, d_timeout_parameter);
  double d_timeout = d_timeout_parameter.as_double();
  rclcpp::Duration timeout = rclcpp::Duration::from_seconds(d_timeout);

  if (!waitForMessage(timeout))
  {
    RCLCPP_ERROR_ONCE(LOGGER,
                      "Could not find parameter %s and did not receive %s via std_msgs::msg::String subscription "
                      "within %f seconds.",
                      name_.c_str(), name_.c_str(), d_timeout);
  }
  if (!keep_open)
  {
    string_subscriber_.reset();
  }
  return content_;
}

bool SynchronizedStringParameter::getMainParameter()
{
  // Check if the parameter is declared, declare it if it's not declared yet
  if (!nodeInterface_->parameter_interface_->has_parameter(name_))
  {
    nodeInterface_->parameter_interface_->declare_parameter(name_, rclcpp::ParameterValue(std::string()));
  }
  rclcpp::Parameter name_parameter;
  nodeInterface_->parameter_interface_->get_parameter(name_, name_parameter);
  content_ = name_parameter.as_string();

  return !content_.empty();
}

bool SynchronizedStringParameter::shouldPublish()
{
  std::string publish_param = "publish_" + name_;
  if (!nodeInterface_->parameter_interface_->has_parameter(publish_param))
  {
    nodeInterface_->parameter_interface_->declare_parameter(publish_param, rclcpp::ParameterValue(false));
  }

  rclcpp::Parameter publish_string_parameter;
  nodeInterface_->parameter_interface_->get_parameter(publish_param, publish_string_parameter);
  bool publish_string = publish_string_parameter.as_bool();

  return publish_string;
}

bool SynchronizedStringParameter::waitForMessage(const rclcpp::Duration timeout)
{
  auto const nd_name = std::string(nodeInterface_->base_node_interface_->get_name()).append("_ssp_").append(name_);
  auto const temp_node = std::make_shared<rclcpp::Node>(nd_name);
  string_subscriber_ = temp_node->create_subscription<std_msgs::msg::String>(
      name_, rclcpp::QoS(1).transient_local().reliable(),
      [this](const std_msgs::msg::String::SharedPtr msg) { return stringCallback(msg); });

  rclcpp::WaitSet wait_set;
  wait_set.add_subscription(string_subscriber_);

  auto ret = wait_set.wait(timeout.to_chrono<std::chrono::duration<double>>());
  if (ret.kind() == rclcpp::WaitResultKind::Ready)
  {
    std_msgs::msg::String msg;
    rclcpp::MessageInfo info;
    if (string_subscriber_->take(msg, info))
    {
      content_ = msg.data;
      return true;
    }
  }
  return false;
}

void SynchronizedStringParameter::stringCallback(const std_msgs::msg::String::SharedPtr msg)
{
  if (msg->data == content_)
  {
    return;
  }
  if (parent_callback_)
  {
    parent_callback_(msg->data);
  }
  content_ = msg->data;
}
}  // namespace rdf_loader
