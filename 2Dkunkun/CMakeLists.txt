cmake_minimum_required(VERSION 3.16)

project(AIkunkun
    VERSION 1.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ---------- dependencies ----------
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 3.3 REQUIRED)
find_package(Eigen3 3.3 REQUIRED NO_MODULE)
find_package(glm CONFIG REQUIRED)       # 当前源码尚使用 GLM
find_package(OpenMP REQUIRED)
find_package(Threads REQUIRED)

if(TARGET glfw)
    set(GLFW_TARGET glfw)
elseif(TARGET glfw3::glfw)
    set(GLFW_TARGET glfw3::glfw)
else()
    message(FATAL_ERROR "找不到 GLFW CMake target")
endif()

function(configure_sim target_name)
    target_link_libraries(${target_name} PRIVATE
        OpenGL::GL
        GLEW::GLEW
        ${GLFW_TARGET}
        Eigen3::Eigen
        glm::glm
        Threads::Threads
    )

    target_compile_options(${target_name} PRIVATE
        -Wall
        -Wextra
        -Wpedantic
    )
endfunction()

# ---------- 3D: 25 parallel environments for training ----------
# add_executable(mma
#     src/mma.cpp
# )

# configure_sim(mma)

# target_link_libraries(mma PRIVATE
#     OpenMP::OpenMP_CXX
# )

# # ---------- 3D: 1 environment for policy playback ----------
# add_executable(mma1
#     src/mma.cpp
# )

# configure_sim(mma1)

# target_link_libraries(mma1 PRIVATE
#     OpenMP::OpenMP_CXX
# )

# target_compile_definitions(mma1 PRIVATE
#     NUM_ENVS=1
# )

# ---------- 2D ----------
add_executable( kunkun2D src/kunkun2D.cpp
)

configure_sim(kunkun2D)