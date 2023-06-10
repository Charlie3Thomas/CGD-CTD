FROM debian:latest

# Install packages
RUN DEBIAN_FRONTEND=noninteractive apt update -qq && apt upgrade -yqq \
    && apt install -yqq sudo build-essential cmake git ninja-build clang clang-tidy clang-format \
       gdb aptitude imagemagick tree libfreeimage-dev libembree-dev libassimp-dev libtinyexr-dev libeigen3-dev

# Add non-root user
ARG USERNAME
RUN groupadd --gid 1000 $USERNAME \
    && useradd --uid 1000 --gid 1000 -s /bin/bash -m $USERNAME \
    && echo $USERNAME ALL=\(root\) NOPASSWD:ALL > /etc/sudoers.d/$USERNAME \
    && chmod 0440 /etc/sudoers.d/$USERNAME
USER $USERNAME
