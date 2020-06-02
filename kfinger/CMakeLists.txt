add_subdirectory(toolbar)

include_directories(
    ${KDE1_INCLUDE_DIR}
)

set(KFINGER_SRCS finger.C kfinger.C)

set(KFINGER_MOC finger.moc kfinger.moc)

QT1_WRAP_MOC(MOC_FILES ${KFINGER_MOC})

add_executable(kfinger ${KFINGER_SRCS} ${MOC_FILES})
target_link_libraries(kfinger
    Qt::Qt1
    ${X11_LIBRARIES}
    ${KDE1_KDECORE}
    ${KDE1_KDEUI}
)

install(TARGETS kfinger RUNTIME DESTINATION ${KDE1_BINDIR})
install(FILES kfinger.kdelnk DESTINATION ${KDE1_APPSDIR}/Internet)
install(FILES kfingerrc DESTINATION ${KDE1_CONFDIR})
