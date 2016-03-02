TEMPLATE = subdirs

SUBDIRS += \
     SpeechKitDemo

macx {
    SUBDIRS += \
        SpeechKitDemoTests
}
