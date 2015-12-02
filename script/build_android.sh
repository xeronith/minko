#!/bin/bash

set -x
set -e

# we save the project's current directory location
CWD=`pwd`

ANDROID_KEYSTORE_PATH="${ANDROID_KEYSTORE_PATH}"
ANDROID_KEYSTORE_ALIAS="${ANDROID_KEYSTORE_ALIAS}"
ANDROID_KEYSTORE_PASSWORD="${ANDROID_KEYSTORE_PASSWORD}"
ANDROID="${ANDROID_HOME}"

ADB="${ANDROID_HOME}/platform-tools/adb"
ZIPALIGN="${ANDROID_HOME}/tools/zipalign"

VERSION_CODE="1"

if ! [ -z "${BUILD_NUMBER}" ]; then
	VERSION_CODE="${BUILD_NUMBER}"
fi

if [ ${OSTYPE} == "cygwin" ]; then
	MINKO_HOME=`cygpath -u "${MINKO_HOME}"`
	ANDROID=`cygpath -u "${ANDROID_HOME}"`
fi

#RSYNC_OPTIONS="--ignore-existing"
TARGET=$1
TARGET_NAME=$(basename ${TARGET})
TARGET_DIR=$(dirname ${TARGET})
CONFIG=$(basename ${TARGET_DIR})
APP_NAME=$(sed -r 's/lib(.*).so/\1/;s/-/ /g;s/([A-Za-z])([A-Za-z]+)/\U\1\L\2/g;s/([0-9]+)//g;s/[^[:alpha:]]\s//g' <<< "${TARGET_NAME}")
PACKAGE=$(sed -r 's/lib(.*).so/com.\1/;s/-/\./g;s/\.([0-9]+)//g;s/(.*)/\L\1/' <<< "${TARGET_NAME}")
ARTIFACT_NAME=$(sed -r 's/ /-/g;s/(.*)/\L\1/' <<< "${APP_NAME}")

pushd "${TARGET_DIR}" > /dev/null

if [ -d "${CWD}/android/" ]; then
	rsync -vr "${CWD}/android/" .
fi

mkdir -p src/${PACKAGE//.//}
mv src/*.java src/${PACKAGE//.//}

sed -i "s/{{APP_NAME}}/${APP_NAME}/" res/values/strings.xml build.xml
sed -i "s/{{PACKAGE}}/${PACKAGE}/" AndroidManifest.xml src/${PACKAGE//.//}/*.java

# Update version info
sed -i "s/{{VERSION_CODE}}/$VERSION_CODE/" AndroidManifest.xml

mkdir -p libs/armeabi-v7a/
# mkdir -p libs/x86/

cp "${TARGET_NAME}" libs/armeabi-v7a/libmain.so
# cp *.so libs/x86/libmain.so

rm -rf assets
mv asset assets
chmod u+rwx -R assets

ant "${CONFIG}"

UNSIGNED_APK_PATH="bin/${APP_NAME}-${CONFIG}-unsigned.apk"
ARTIFACT_PATH="bin/${ARTIFACT_NAME}-${CONFIG}.apk"

DEVICE_STATE=$("${ADB}" get-state | sed 's/\r$//')

if [[ "${CONFIG}" == release && -n "${ANDROID_KEYSTORE_PATH}" && -n "${ANDROID_KEYSTORE_PASSWORD}" && -n "${ANDROID_KEYSTORE_ALIAS}" ]]; then
	# Sign the app
	jarsigner -tsa "http://timestamp.digicert.com" -keystore "${ANDROID_KEYSTORE_PATH}" -storepass "${ANDROID_KEYSTORE_PASSWORD}" -verbose -sigalg SHA1withRSA -digestalg SHA1 "${UNSIGNED_APK_PATH}" "${ANDROID_KEYSTORE_ALIAS}"

	# Verify that the app is properly signed
	jarsigner -verify -verbose -certs "${UNSIGNED_APK_PATH}"

	# zipalign ensures that all uncompressed data starts with a particular byte alignment relative to the start of the file,
	# which reduces the amount of RAM consumed by an app.
	"${ZIPALIGN}" -v -f 4 "${UNSIGNED_APK_PATH}" "${ARTIFACT_PATH}"

	rm -f "${UNSIGNED_APK_PATH}"

	# Don't forget to uninstall the app to avoid INSTALL_PARSE_FAILED_INCONSISTENT_CERTIFICATES error
	if [[ "${DEVICE_STATE}" == "device" ]]; then
		"${ADB}" uninstall $PACKAGE
	fi
else
	mv "${UNSIGNED_APK_PATH}" "${ARTIFACT_PATH}"
fi

if [ "${DEVICE_STATE}" == "device" ]; then
	"${ADB}" install -r "${ARTIFACT_PATH}"
fi

popd > /dev/null
