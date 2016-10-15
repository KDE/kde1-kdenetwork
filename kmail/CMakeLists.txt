add_subdirectory(pics)

include_directories(
    ${QT_INCLUDE_DIR}
    ${KDE1_INCLUDE_DIR}
)

set(KMAIL_SRCS
    charsetsDlg.cpp kalarmtimer.cpp kbusyptr.cpp kfileio.cpp kfontutils.cpp
    kmaccount.cpp kmacctfolder.cpp kmacctlocal.cpp kmacctmgr.cpp kmacctpop.cpp
    kmacctseldlg.cpp kmaddrbook.cpp kmaddrbookdlg.cpp kmcomposewin.cpp
    kmfilteraction.cpp kmfilter.cpp kmfilterdlg.cpp kmfiltermgr.cpp kmfolder.cpp
    kmfolderdia.cpp kmfolderdir.cpp kmfoldermgr.cpp kmfoldernode.cpp
    kmfolderseldlg.cpp kmfoldertree.cpp kmheaders.cpp kmidentity.cpp kmimemagic.cpp
    kmiostatusdlg.cpp kmmainwin.cpp kmmessage.cpp kmmsgbase.cpp kmmsginfo.cpp
    kmmsglist.cpp kmmsgpart.cpp kmmsgpartdlg.cpp kmnewiostatus.cpp
    kmnewiostatuswdg.cpp kmreaderwin.cpp kmsender.cpp kmsettings.cpp
    kmtopwidget.cpp kpgpbase.cpp kpgp.cpp main.cpp
)

set(KMAIL_MOC
    charsetsDlg.moc kalarmtimer.moc kbusyptr.moc kmaccount.moc
    kmacctmgr.moc kmacctpop.moc kmacctseldlg.moc kmaddrbookdlg.moc
    kmcomposewin.moc kmfilteraction.moc kmfilterdlg.moc kmfolderdia.moc
    kmfolderdir.moc kmfolder.moc kmfoldermgr.moc kmfoldernode.moc
    kmfolderseldlg.moc kmfoldertree.moc kmheaders.moc kmiostatusdlg.moc
    kmmainwin.moc kmmsgbase.moc kmmsgpartdlg.moc kmnewiostatus.moc
    kmnewiostatuswdg.moc kmreaderwin.moc kmsender.moc kmsettings.moc
    kmtopwidget.moc kpgp.moc
)

QT1_WRAP_MOC(MOC_FILES ${KMAIL_MOC})

add_executable(kmail ${KMAIL_SRCS} ${MOC_FILES})
target_link_libraries(kmail
    ${QT_LIBRARIES}
    ${X11_LIBRARIES}
    ${KDE1_KDECORE}
    ${KDE1_KDEUI}
    ${KDE1_KHTMLW}
    ${KDE1_KFILE}
    ${KDE1_JSCRIPT}
    ${KDE1_KFM}
    ${KDE1_KIMGIO}
    ${KDE1_KSPELL}
    mimelib
)

install(TARGETS kmail RUNTIME DESTINATION ${KDE1_BINDIR})

install(FILES KMail.kdelnk DESTINATION ${KDE1_DATADIR}/kmail)
