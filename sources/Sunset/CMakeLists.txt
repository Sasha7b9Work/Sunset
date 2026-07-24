# 2023/07/04 15:47:47 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
project (Sunset)

set(CMAKE_UNITY_BUILD ON)

set(CMAKE_UNITY_BUILD_BATCH_SIZE 20)

if(WIN32)

    include_directories(
        src
        arm64_plugs
        linux_plugs
        ../ThirdParty/wxWidgets/include
        ../generated/ThirdParty/lib/vc_x64_lib/mswu
    )

    link_directories(../generated/ThirdParty/lib/vc_x64_lib)

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -MP -Wall -std:c++20")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS /STACK:10000000")

    add_compile_options(
        # Обязательные
        /wd4265    # class has virtual functions, but its non-trivial destructor is not virtual
        /wd4355    # 'this' : used in base member initializer list
        /wd4514    # unreferenced inline function has been removed
        /wd4625    # copy constructor was implicitly defined as deleted
        /wd4623    # default constructor was implicitly defined as deleted
        /wd4626    # assignment operator was implicitly defined as deleted
        /wd4710    # function not inlined
        /wd4711    # function xxx selected for automatic inline expansion
        /wd4820    # 'N' bytes padding added after data member
        /wd4865    # '<unnamed-enum-XXX>': the underlying type will change from 'int' to '__int64' when '/Zc:enumTypes' is specified on the command line
        /wd5026    # move constructor was implicitly defined as deleted
        /wd5027    # move assignment operator was implicitly defined as deleted
        /wd5045    # Compiler will insert Spectre mitigation for memory load if /Qspectre switch specified
        /wd5039    # pointer or reference to potentially throwing function passed to extern C function under -EHc. Undefined behavor may occur if this function throws an exception
        /wd5233    # xplicit lambda capture 'this' is not used
    )

else()

    if(CMAKE_SYSTEM_PROCESSOR MATCHES "aarch64")
        include_directories(
            ./src
            ../generated/wxWidgets/lib/wx/include/gtk3-unicode-static-3.3
            ../ThirdParty/wxWidgets/include
        )
    else()
        include_directories(
            ./arm64_plugs
            ./src
            ../generated/wxWidgets/lib/wx/include/gtk3-unicode-static-3.3
            ../ThirdParty/wxWidgets/include
        )
    endif()

    link_directories(
        ../generated/wxWidgets/lib
    )

    add_definitions(-D__WXGTK__ -D__WXGTK3__ -D_FILE_OFFSET_BITS=64)

endif()

add_definitions(-DOPT_GENERIC)

if(WIN32)
    set(RapidJSON_DIR "${CMAKE_CURRENT_LIST_DIR}/../../vcpkg_installed/x64-windows/share/rapidjson" CACHE PATH "Path to RapidJSON config")
else()
    if(CMAKE_SYSTEM_PROCESSOR MATCHES "aarch64")
        set(RapidJSON_DIR "${CMAKE_CURRENT_LIST_DIR}/../../vcpkg_installed/arm64-linux/share/rapidjson" CACHE PATH "Path to RapidJSON config")
    else()
        set(RapidJSON_DIR "${CMAKE_CURRENT_LIST_DIR}/../../vcpkg_installed/x64-linux/share/rapidjson" CACHE PATH "Path to RapidJSON config")
    endif()
endif()

file(GLOB RESOURCES                     resources/*.*)
file(GLOB SRC                           src/*.*)
file(GLOB COMMUNICATOR                  src/Communicator/*.*)
file(GLOB COMMUNICATOR_COMPORT          src/Communicator/ComPort/*.*)
file(GLOB COMMUNICATOR_GPIO             src/Communicator/GPIO/*.*)
file(GLOB COMMUNICATOR_SPI              src/Communicator/SPI/*.*)
file(GLOB COMMUNICATOR_UART             src/Communicator/UART/*.*)
file(GLOB CONTROLS                      src/Controls/*.*)
file(GLOB DATA                          src/Data/*.*)
file(GLOB IPPP                          src/IPPP/*.*)
file(GLOB IPPP_EMULATOR                 src/IPPP/Emulator/*.*)
file(GLOB IPPP_KEYBOARD                 src/IPPP/Keyboard/*.*)
file(GLOB IPPP_REAL                     src/IPPP/Real/*.*)
file(GLOB IPPP_SCPI                     src/IPPP/SCPI/*.*)
file(GLOB MAIN_PANEL                    src/MainPanel/*.*)
file(GLOB PANEL_ARCHIVE                 src/Panels/PanelArchive/*.*)
file(GLOB PANEL_DEBUG                   src/Panels/PanelDebug/*.*)
file(GLOB PANEL_DEBUG_NOTEBOOK          src/Panels/PanelDebug/Notebook/*.*)
file(GLOB PANEL_MEAS                    src/Panels/PanelMeasures/*.*)
file(GLOB PANEL_MEAS_GRAPHICS           src/Panels/PanelMeasures/Graphics/*.*)
file(GLOB PANEL_MEAS_GRID               src/Panels/PanelMeasures/Grid/*.*)
file(GLOB PANEL_REPORTS                 src/Panels/PanelReports/*.*)
file(GLOB PANEL_SETTINGS                src/Panels/PanelSettings/*.*)
file(GLOB PANEL_TABLES                  src/Panels/PanelTables/*.*)
file(GLOB PAGE_TESTS                    src/Panels/PageTests/*.*)
file(GLOB SETTINGS                      src/Settings/*.*)
file(GLOB SETTINGS_TESTS                src/Settings/Tests/*.*)
file(GLOB SETTINGS_TESTS_LIBRARY        src/Settings/Tests/Library/*.*)
file(GLOB SOFTTESTS                     src/SoftTests/*.*)
file(GLOB STORAGE                       src/Storage/*.*)
file(GLOB SYSTEM                        src/System/*.*)
file(GLOB UTILS                         src/Utils/*.*)
file(GLOB WINDOWS                       src/Windows/*.*)

if(WIN32)                                                           # Эти заглушки включаем для Windows
    file(GLOB PLUGS_ARM64               arm64_plugs/*.*
                                        arm64_plugs/linux/spi/*.*)
    file(GLOB PLUGS_LINUX               linux_plugs/*.*
                                        linux_plugs/sys/*.*
                                        linux_plugs/libssh/*.*)
else()
    if(CMAKE_SYSTEM_PROCESSOR MATCHES "aarch64")
    else()                                                          # Эти заглушки включаем для Ubuntu x64
        file(GLOB PLUGS_ARM64           arm64_plugs/*.*
                                        arm64_plugs/linux/spi/*.*)
    endif()
endif()

source_group(Resources                        FILES ${RESOURCES})
source_group(src                              FILES ${SRC})
source_group(Communicator                     FILES ${COMMUNICATOR})
source_group(Communicator/ComPort             FILES ${COMMUNICATOR_COMPORT})
source_group(Communicator/GPIO                FILES ${COMMUNICATOR_GPIO})
source_group(Communicator/SPI                 FILES ${COMMUNICATOR_SPI})
source_group(Communicator/UART                FILES ${COMMUNICATOR_UART})
source_group(Controls                         FILES ${CONTROLS})
source_group(Data                             FILES ${DATA})
source_group(IPPP                             FILES ${IPPP})
source_group(IPPP/Emulator                    FILES ${IPPP_EMULATOR})
source_group(IPPP/Keyboard                    FILES ${IPPP_KEYBOARD})
source_group(IPPP/Real                        FILES ${IPPP_REAL})
source_group(IPPP/SCPI                        FILES ${IPPP_SCPI})
source_group(Panels                           FILES ${MAIN_PANEL})
source_group(Panels/PanelArchive              FILES ${PANEL_ARCHIVE})
source_group(Panels/PanelDebug                FILES ${PANEL_DEBUG})
source_group(Panels/PanelDebug/Notebook       FILES ${PANEL_DEBUG_NOTEBOOK})
source_group(Panels/PanelMeasures             FILES ${PANEL_MEAS})
source_group(Panels/PanelMeasures/Graphics    FILES ${PANEL_MEAS_GRAPHICS})
source_group(Panels/PanelMeasures/Grid        FILES ${PANEL_MEAS_GRID})
source_group(Panels/PanelReports              FILES ${PANEL_REPORTS})
source_group(Panels/PanelSettings             FILES ${PANEL_SETTINGS})
source_group(Panels/PanelTables               FILES ${PANEL_TABLES})
source_group(Panels/PageTests                 FILES ${PAGE_TESTS})
source_group(Settings                         FILES ${SETTINGS})
source_group(Settings/Tests                   FILES ${SETTINGS_TESTS})
source_group(Settings/Tests/Library           FILES ${SETTINGS_TESTS_LIBRARY})
source_group(SoftTests                        FILES ${SOFTTESTS})
source_group(Storage                          FILES ${STORAGE})
source_group(System                           FILES ${SYSTEM})
source_group(Utils                            FILES ${UTILS})
source_group(Windows                          FILES ${WINDOWS})
source_group(Plugs/Arm64                      FILES ${PLUGS_ARM64})
source_group(Plugs/linux                      FILES ${PLUGS_LINUX})

find_package(RapidJSON CONFIG REQUIRED)

add_executable(${PROJECT_NAME}
    ${RESOURCES}
    ${SRC}
    ${COMMUNICATOR}
    ${COMMUNICATOR_COMPORT}
    ${COMMUNICATOR_GPIO}
    ${COMMUNICATOR_SPI}
    ${COMMUNICATOR_UART}
    ${CONTROLS}
    ${DATA}
    ${IPPP}
    ${IPPP_EMULATOR}
    ${IPPP_KEYBOARD}
    ${IPPP_REAL}
    ${IPPP_SCPI}
    ${MAIN_PANEL}
    ${PANEL_ARCHIVE}
    ${PANEL_DEBUG}
    ${PANEL_DEBUG_NOTEBOOK}
    ${PANEL_MEAS}
    ${PANEL_MEAS_GRAPHICS}
    ${PANEL_MEAS_GRID}
    ${PANEL_REPORTS}
    ${PANEL_SETTINGS}
    ${PANEL_TABLES}
    ${PAGE_TESTS}
    ${SETTINGS}
    ${SETTINGS_TESTS}
    ${SETTINGS_TESTS_LIBRARY}
    ${SOFTTESTS}
    ${STORAGE}
    ${SYSTEM}
    ${UTILS}
    ${WINDOWS}
    ${PLUGS_ARM64}
    ${PLUGS_LINUX}
)

set_source_files_properties(
    src/version.cpp
    PROPERTIES SKIP_UNITY_BUILD_INCLUSION ON
)


if(WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE
        RapidJSON rapidjson
        wxbase33u
        wxmsw33u_core
        wxbase33u_net
        wxzlib
        wxpng
        Ws2_32
        Comctl32
        Rpcrt4
        Setupapi
        Iphlpapi
    )

    # Копируем в рабочий каталог
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND copy $(OutputPath)\\$(PROJECT_NAME) . /Y)
else()
    find_package(PkgConfig REQUIRED)

    pkg_check_modules(GTK3 REQUIRED gtk+-3.0)

    if(CMAKE_SYSTEM_PROCESSOR MATCHES "aarch64")
        pkg_check_modules(GPIOD REQUIRED libgpiod)

        if(NOT GPIOD_FOUND)
            message(FATAL_ERROR "libgpiod is required for ARM64 builds but was not found! Install with: sudo apt-get install libgpiod-dev")
        endif()
        
        target_compile_definitions(${PROJECT_NAME} PRIVATE ARM64)
        target_link_libraries(${PROJECT_NAME} ${GPIOD_LIBRARIES})
        target_include_directories(${PROJECT_NAME} PRIVATE ${GPIOD_INCLUDE_DIRS})
        target_compile_options(${PROJECT_NAME} PRIVATE ${GPIOD_CFLAGS_OTHER})
    else()
        message(STATUS "Non-ARM64 platform detected (${CMAKE_SYSTEM_PROCESSOR}) - skipping libgpiod search")
    endif()

    pkg_check_modules(GSPELL gspell-1)

    target_link_libraries(${PROJECT_NAME}
        RapidJSON rapidjson
        wx_gtk3u_core-3.3
        wx_baseu-3.3
        wx_baseu_net-3.3
        X11
        xkbcommon
        png
        ssh
        ${GTK3_LIBRARIES}
        ${GSPELL_LIBRARIES}
        wayland-client
    )

    target_include_directories(${PROJECT_NAME} PRIVATE ${GTK3_INCLUDE_DIRS})
    target_compile_options(${PROJECT_NAME} PRIVATE ${GTK3_CFLAGS_OTHER})

endif()


