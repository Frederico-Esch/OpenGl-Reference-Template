cmake_minimum_required(VERSION 3.20)
project(opengl_template)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "/W4 /WX")

add_executable(opengl_template "${CMAKE_CURRENT_SOURCE_DIR}/src/gl.c" "${CMAKE_CURRENT_SOURCE_DIR}/src/main.c")

target_include_directories(opengl_template PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_link_libraries(opengl_template PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/glfw3dll.lib" PRIVATE opengl32.lib PRIVATE User32.lib PRIVATE Gdi32.lib PRIVATE Shell32.lib)


#DLL
add_custom_command(
    TARGET opengl_template POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    "${CMAKE_CURRENT_SOURCE_DIR}/glfw3.dll"
    $<TARGET_FILE_DIR:opengl_template>
)

#graphics.config
add_custom_command(
    TARGET opengl_template POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    "${CMAKE_CURRENT_SOURCE_DIR}/graphics.config"
    $<TARGET_FILE_DIR:opengl_template> #"${CMAKE_BINARY_DIR}"
)

#SHADERS
#add_custom_command(
#    TARGET opengl_template POST_BUILD
#    COMMAND ${CMAKE_COMMAND} -E make_directory
#    "${CMAKE_CURRENT_SOURCE_DIR}/shaders"
#    $<TARGET_FILE_DIR:opengl_template>
#)
add_custom_command(
    TARGET opengl_template POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E create_symlink
    "${CMAKE_CURRENT_SOURCE_DIR}/shaders"
    "$<TARGET_FILE_DIR:opengl_template>/shaders" #"${CMAKE_BINARY_DIR}/shaders"
)

#TEXTURES
#add_custom_command(
#    TARGET opengl_template POST_BUILD
#    COMMAND ${CMAKE_COMMAND} -E make_directory
#    "$<TARGET_FILE_DIR:opengl_template>/textures"
#)
add_custom_command(
    TARGET opengl_template POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E create_symlink
    "${CMAKE_CURRENT_SOURCE_DIR}/textures"
    "$<TARGET_FILE_DIR:opengl_template>/textures" #"${CMAKE_BINARY_DIR}/textures"
)
