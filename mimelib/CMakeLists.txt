include_directories(
    ${QT_INCLUDE_DIR}
)

set(MIMELIB_SRCS
    address.cpp addrlist.cpp attach.cpp basicmsg.cpp
    binhex.cpp body.cpp bodypart.cpp boyermor.cpp
    datetime.cpp disptype.cpp dw_cte.cpp dw_date.cpp
    dw_mime.cpp entity.cpp fieldbdy.cpp field.cpp
    group.cpp headers.cpp mailbox.cpp mboxlist.cpp
    mediatyp.cpp mechansm.cpp message.cpp msgcmp.cpp
    msgid.cpp multipar.cpp nntp.cpp param.cpp pop.cpp
    protocol.cpp smtp.cpp string.cpp text.cpp token.cpp
    uuencode.cpp
    )

add_library(mimelib SHARED ${MIMELIB_SRCS})
set_target_properties(mimelib PROPERTIES VERSION 1.0 SOVERSION 1)
install(TARGETS mimelib LIBRARY DESTINATION ${KDE1_LIBDIR})

install(DIRECTORY mimelib DESTINATION ${KDE1_INCLUDES})
