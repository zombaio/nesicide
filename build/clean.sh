#!/bin/bash

echo Building NESICIDE...
( cd ide; make distclean )
echo Building FamiTracker...
( cd famitracker; make distclean )
echo Building FamiPlayer...
( cd famiplayer; make distclean )
echo Building NES Emulator...
( cd nes-emulator; make distclean )

