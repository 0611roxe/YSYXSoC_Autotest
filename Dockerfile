FROM ubuntu:22.04

# Set the working directory
WORKDIR /ysyx-local-autotest

# Install necessary dependencies
RUN apt-get update && \
    apt-get install -y \
    python3-pip \
    git \
    help2man \
    perl \
    python3 \
    make \
    g++ \
    build-essential \
    g++-riscv64-linux-gnu \
    binutils-riscv64-linux-gnu \
    libsdl2-dev \
    libsdl2-image-dev \
    libsdl2-ttf-dev \
    libreadline-dev \
    autoconf \
    flex \
    bison \
    libfl2 \
    libfl-dev \
    libgoogle-perftools-dev \
    ccache \
    libperl-dev \
    perl-doc \
    libxml2-utils \
    xdg-utils \
    && rm -rf /var/lib/apt/lists/*

# # Install JAVA JDK
# RUN apt-get update && \
#     apt-get install -y openjdk-11-jdk && \
#     rm -rf /var/lib/apt/lists/*

# # Install Scala 2.13.14
# RUN apt-get update && \
#     apt-get install -y scala && \
#     rm -rf /var/lib/apt/lists/*

# # Install sbt(if needed)
# RUN curl -fsSL https://repo.scala-sbt.org/scalasbt/gpg/key | gpg --dearmor > /usr/share/keyrings/sbt-keyring.gpg && \
#     echo "deb [signed-by=/usr/share/keyrings/sbt-keyring.gpg] https://repo.scala-sbt.org/scalasbt/debian all main" | tee /etc/apt/sources.list.d/sbt.list && \
#     apt-get update && \
#     apt-get install -y sbt && \
#     rm -rf /var/lib/apt/lists/*

# # Install mill 0.12.4
# RUN wget https://github.com/com-lihaoyi/mill/releases/download/0.12.4/0.12.4 && \
#     mv 0.12.4 /usr/local/bin/mill && \
#     chmod +x /usr/local/bin/mill

# # Configure JAVA_HOME(if needed)
# ENV JAVA_HOME=/usr/lib/jvm/java-11-openjdk-amd64
# ENV PATH=$JAVA_HOME/bin:$PATH

# Install verilator
# RUN git clone https://github.com/verilator/verilator.git /tmp/verilator && \
RUN git clone https://gitee.com/mirrors/Verilator.git /tmp/verilator && \
    cd /tmp/verilator && \
    git checkout stable && \
    autoconf && \
    ./configure && \
    make -j$(nproc) && \
    make install && \
    cd /ysyx-local-autotest && \
    rm -rf /tmp/verilator

# Install pyyaml
RUN pip3 install -i https://mirrors.tuna.tsinghua.edu.cn/pypi/web/simple \
    pyyaml

# fix the compilation error of application using riscv64-linux-gnu toolchain
RUN sed -i 's|^# include <gnu/stubs-ilp32.h>|//# include <gnu/stubs-ilp32.h>|' /usr/riscv64-linux-gnu/include/gnu/stubs.h

# Copy the scripts and entrypoint
COPY scripts/ /ysyx-local-autotest/scripts/
RUN chmod +x /ysyx-local-autotest/scripts/*.py

# Copy the test framework
COPY ysyx-local-autotest/test-framework/ /ysyx-local-autotest/test-framework/

