set(ROTATION_SENSOR_DIR ${CMAKE_CURRENT_LIST_DIR})

set(ROTATION_SENSOR_SRCS ${ROTATION_SENSOR_DIR}/src/rotation_sensor_v2.c)

idf_component_register(SRCS ${ROTATION_SENSOR_SRCS}
                       INCLUDE_DIRS ${ROTATION_SENSOR_DIR}/inc
                       REQUIRES esp_adc)