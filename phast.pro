TEMPLATE = subdirs
SUBDIRS += phast_gui timetrace_view_plugin fake_timetag_device_plugin qutau_timetag_plugin qutag_timetag_plugin decaycurve_view_plugin correlation_view_plugin
CONFIG += ordered

timetrace_view_plugin.depends = phast_gui
fake_timetag_device_plugin.depends = phast_gui
qutau_timetag_plugin.depends = phast_gui
qutag_timetag_plugin.depends = phast_gui
decaycurve_view_plugin.depends = phast_gui
correlation_view_plugin.depends = phast_gui
