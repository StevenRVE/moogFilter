cmake_minimum_required(VERSION 3.7)
 project(sve-moogFilter)

 add_subdirectory(dpf)

 dpf_add_plugin(sve-moogFilter
   TARGETS clap lv2 vst2 vst3

   FILES_DSP
       source/moogFilter.cpp
 )

 target_include_directories(sve-moogFilter
   PUBLIC source)

 target_compile_options(sve-moogFilter PUBLIC "-Wno-c++11-narrowing")

 add_compile_options(-fdiagnostics-color=always)