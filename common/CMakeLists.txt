SET(PHN_DBUS "phn-dbus")

FIND_PROGRAM(GDBUS_CODEGEN NAMES gdbus-codegen)

ADD_CUSTOM_COMMAND(
	OUTPUT dbus
	COMMAND ${GDBUS_CODEGEN} --generate-c-code ${PHN_DBUS}
			--interface-prefix org.tizen.PhonenumberUtils.
			--c-namespace phn
			${PHN_DBUS}.xml
	DEPENDS ${PHN_DBUS}.xml)

ADD_CUSTOM_TARGET(GENERATED_DBUS_CODE DEPENDS dbus)
