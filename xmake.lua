add_rules("mode.debug", "mode.release")

set_project("QtWidgetTest")

if is_plat("windows") then
    set_languages("cxxlatest")

    -- bruh
    add_cxflags("/Zc:__cplusplus")
    add_cxflags("/Zc:preprocessor")
else
    set_languages("cxx20")
end

set_warnings("allextra")

if is_mode("debug") then
    add_defines("DEBUG")

    set_symbols("debug")

    set_optimize("fastest")
end

if is_mode("release") then
    add_defines("NDEBUG")

    add_cxflags("-fomit-frame-pointer")

    set_optimize("fastest")
end

target("QtWidgetTest")
    add_rules("qt.widgetapp")
    add_frameworks("QtNetwork")
    add_frameworks("QtXml")

    add_headerfiles("src/**.h")
    add_files("src/**.cpp")
    add_files("src/mainwindow.ui")
    -- add files with Q_OBJECT meta (only for qt.moc)
    add_files("src/filedownloader.h")
    add_files("src/mainwindow.h")
