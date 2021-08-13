# moveit/moveit2:foxy-release
# Full debian-based install of MoveIt using apt-get

ARG ROS_DISTRO=foxy
FROM ros:${ROS_DISTRO}-ros-base-focal
MAINTAINER Dave Coleman dave@picknik.ai

# Commands are combined in single RUN statement with "apt/lists" folder removal to reduce image size
RUN apt-get update && \
    git config --global user.email "you@example.com" && \
    git config --global user.name "Your Name" && \
    apt-get install -y ros-${ROS_DISTRO}-moveit-* && \
    rm -rf /var/lib/apt/lists/*
