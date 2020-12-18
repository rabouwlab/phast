#!/bin/sh

buildDir=./build/
deployDir=./deployed/
appProjectName=phast_gui
appName=phast.exe
buildMode=release

# Add any additional plugins that can be placed in the general "plugins" folder here
plugins=("decaycurve_view_plugin" "correlation_view_plugin" "fake_timetag_device_plugin" "timetrace_view_plugin")


mkdir ${deployDir}
cp ${buildDir}${appProjectName}/${buildMode}/${appName} ${deployDir}

# do Qt deployment
windeployqt ${deployDir}${appName}

# Find dependencies
deps=$(ntldd -R ${deployDir}${appName} | grep -v 'C:\\WINDOWS\\' | grep -v ext-ms- | grep -v "not found" | awk '{print $1}')

depsLocs=$(which ${deps})

# Filter out dependencies provided by windows
depsFiltered=$(echo ${depsLocs} | grep -v 'Windows/System32')

# Copy in dependencies
for dep in $depsFiltered
do
	cp ${dep} ${deployDir}
done

# Copy plugins
mkdir ${deployDir}/plugins

for pl in "${plugins[@]}"
do
	cp ./build/${pl}/${buildMode}/${pl}.dll ${deployDir}/plugins
	
	# Find plugin dependencies
	depsTemp=$(ntldd -R ${deployDir}/plugins/${pl}.dll | grep -v 'C:\\WINDOWS\\' | grep -v ext-ms- | grep -v "not found" | awk '{print $1}')

	depsLocsTemp=$(which ${depsTemp})

	# Filter out dependencies provided by windows
	depsFilteredTemp=$(echo ${depsLocsTemp} | grep -v 'Windows/System32')

	# Copy in dependencies
	for dep in $depsFilteredTemp
	do
		cp ${dep} ${deployDir}
	done
done

# Copy quTAG plugin
mkdir ${deployDir}/qutag_plugin
cp ./qutag_timetag_plugin/qutag_bin/tdcbase.dll ${deployDir}/qutag_plugin/
cp ./qutag_timetag_plugin/qutag_bin/FTD3XX.dll ${deployDir}/qutag_plugin/
cp ./qutag_timetag_plugin/qutag_bin/libusb0.dll ${deployDir}/qutag_plugin/
cp ./build/qutag_timetag_plugin/release/qutag_timetag_plugin.dll ${deployDir}/qutag_plugin/

# Copy quTAU plugin
mkdir ${deployDir}/qutau_plugin
cp ./qutau_timetag_plugin/qutau_bin/tdcbase.dll ${deployDir}/qutau_plugin/
cp ./qutau_timetag_plugin/qutau_bin/libusb0.dll ${deployDir}/qutau_plugin/
cp ./build/qutau_timetag_plugin/release/qutau_timetag_plugin.dll ${deployDir}/qutau_plugin/

# Create Bat files to start using qutag or qutau plugins
echo "phast.exe -P qutag_plugin" > ./deployed/phast_qutag.bat
echo "phast.exe -P qutau_plugin" > ./deployed/phast_qutau.bat

# Copy in readme and license file
cp LICENSE ${deployDir}license.txt
cp app_readme.txt ${deployDir}readme.txt