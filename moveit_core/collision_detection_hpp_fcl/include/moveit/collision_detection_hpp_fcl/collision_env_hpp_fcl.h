/*********************************************************************
 * Software License Agreement (BSD License)
 *
 *  Copyright (c) 2011, Willow Garage, Inc.
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
 *   * Neither the name of the copyright holder nor the names of its
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

/* Author: Ioan Sucan, Jens Petit */

#pragma once

#include <moveit/collision_detection/collision_env.h>
#include <moveit/collision_detection_hpp_fcl/collision_common.h>

#include <hpp/fcl/broadphase/broadphase_collision_manager.h>

#include <memory>

namespace collision_detection
{
/** \brief HPPFCL implementation of the CollisionEnv */
class CollisionEnvHPPFCL : public CollisionEnv
{
public:
  CollisionEnvHPPFCL() = delete;

  CollisionEnvHPPFCL(const moveit::core::RobotModelConstPtr& model, double padding = 0.0, double scale = 1.0);

  CollisionEnvHPPFCL(const moveit::core::RobotModelConstPtr& model, const WorldPtr& world, double padding = 0.0,
                     double scale = 1.0);

  CollisionEnvHPPFCL(const CollisionEnvHPPFCL& other, const WorldPtr& world);

  ~CollisionEnvHPPFCL() override;

  void checkSelfCollision(const CollisionRequest& req, CollisionResult& res,
                          const moveit::core::RobotState& state) const override;

  void checkSelfCollision(const CollisionRequest& req, CollisionResult& res, const moveit::core::RobotState& state,
                          const AllowedCollisionMatrix& acm) const override;

  void checkRobotCollision(const CollisionRequest& req, CollisionResult& res,
                           const moveit::core::RobotState& state) const override;

  void checkRobotCollision(const CollisionRequest& req, CollisionResult& res, const moveit::core::RobotState& state,
                           const AllowedCollisionMatrix& acm) const override;

  void checkRobotCollision(const CollisionRequest& req, CollisionResult& res, const moveit::core::RobotState& state1,
                           const moveit::core::RobotState& state2, const AllowedCollisionMatrix& acm) const override;

  void checkRobotCollision(const CollisionRequest& req, CollisionResult& res, const moveit::core::RobotState& state1,
                           const moveit::core::RobotState& state2) const override;

  void distanceSelf(const DistanceRequest& req, DistanceResult& res,
                    const moveit::core::RobotState& state) const override;

  void distanceRobot(const DistanceRequest& req, DistanceResult& res,
                     const moveit::core::RobotState& state) const override;

  void setWorld(const WorldPtr& world) override;

protected:
  /** \brief Updates the HPPFCL collision geometry and objects saved in the CollisionRobotHPPFCL members to reflect a
   * new padding or scaling of the robot links.
   *
   *   It searches for the link through the pointed-to robot model of the CollisionRobot and then constructs new HPPFCL
   *   collision objects and geometries depending on the changed robot model.
   *
   *   \param links The names of the links which have been updated in the robot model */
  void updatedPaddingOrScaling(const std::vector<std::string>& links) override;

  /** \brief Bundles the different checkSelfCollision functions into a single function */
  void checkSelfCollisionHelper(const CollisionRequest& req, CollisionResult& res,
                                const moveit::core::RobotState& state, const AllowedCollisionMatrix* acm) const;

  /** \brief Bundles the different checkRobotCollision functions into a single function */
  void checkRobotCollisionHelper(const CollisionRequest& req, CollisionResult& res,
                                 const moveit::core::RobotState& state, const AllowedCollisionMatrix* acm) const;

  /** \brief Construct an HPPFCL collision object from MoveIt's World::Object. */
  void constructHPPFCLObjectWorld(const World::Object* obj, HPPFCLObject& fcl_obj) const;

  /** \brief Updates the specified object in \m fcl_objs_ and in the manager from new data available in the World.
   *
   *  If it does not exist in world, it is deleted. If it's not existing in \m fcl_objs_ yet, it's added there. */
  void updateHPPFCLObject(const std::string& id);

  /** \brief Out of the current robot state and its attached bodies construct an HPPFCLObject which can then be used to
   *   check for collision.
   *
   *   The current state is used to recalculate the AABB of the HPPFCL collision objects. However they are not computed from
   *   scratch (which would require call to computeLocalAABB()) but are only transformed according to the joint states.
   *
   *   \param state The current robot state
   *   \param fcl_obj The newly filled object */
  void constructHPPFCLObjectRobot(const moveit::core::RobotState& state, HPPFCLObject& fcl_obj) const;

  /** \brief Prepares for the collision check through constructing an HPPFCL collision object out of the current robot
   *   state and specifying a broadphase collision manager of HPPFCL where the constructed object is registered to. */
  void allocSelfCollisionBroadPhase(const moveit::core::RobotState& state, HPPFCLManager& manager) const;

  /** \brief Converts all shapes which make up an attached body into a vector of HPPFCLGeometryConstPtr.
   *
   *   When they are converted, they can be added to the HPPFCL representation of the robot for collision checking.
   *
   *   \param ab Pointer to the attached body
   *   \param geoms Output vector of geometries
   */
  void getAttachedBodyObjects(const moveit::core::AttachedBody* ab, std::vector<HPPFCLGeometryConstPtr>& geoms) const;

  /** \brief Vector of shared pointers to the HPPFCL geometry for the objects in fcl_objs_. */
  std::vector<HPPFCLGeometryConstPtr> robot_geoms_;

  /** \brief Vector of shared pointers to the HPPFCL collision objects which make up the robot */
  std::vector<HPPFCLCollisionObjectConstPtr> robot_fcl_objs_;

  /// HPPFCL collision manager which handles the collision checking process
  std::unique_ptr<hpp::fcl::BroadPhaseCollisionManager> manager_;

  std::map<std::string, HPPFCLObject> fcl_objs_;

private:
  /** \brief Callback function executed for each change to the world environment */
  void notifyObjectChange(const ObjectConstPtr& obj, World::Action action);

  World::ObserverHandle observer_handle_;
};
}  // namespace collision_detection
