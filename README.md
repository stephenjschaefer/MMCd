MMCd
Mitsubishi ALDL (OBDI) datalogger

Copyright (c) 2003, Dmitry Yurtaev <dm1try@umail.ru>

This is free software; you can redistribute it and/or modify it under the
terms of the GNU General Public License as published by the Free Software
Foundation; either version 2, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

###SETTING UP DEV ENVIRONMENT IN WINDOWS
1. Cygwin Install
  - Download and 32 bit Cygwin installer. (setup-x86.exe)
  - Execute installer via command line with the -X argument (setup-x86.exe -X)
  - Add and select "http://prc-tools.sourceforge.net/install" as a download site.
  - Multiselect another download site for the base Cygwin packages.
  - Install the latest versions of the following packages in the Cygwin installer:
    - prc-tools: GCC for Palm OS and related tools
    - prc-tools:-arm: GCC for Palm OS on ARM (armlets)
    - prc-tools-htmldocs: Documentation for prc-tools, GCC, etc as HTML
    - pilrc: Palm OS resource compiler
    - make: The GNU version of the 'make' utility
	
2. Get Source
  - Clone MMCd repo to a directory on your local machine (C:\PalmDev for example)

3. Configure Cygwin
  - Launch Cygwin emulation window and register the SDK mount and paths using the following commands:
    - $ mount -f "C:\PalmDev\MMCd" /PalmDev
    - $ palmdev-prep

4. Build MMCd
  - Launch Cygwin emulation window and build MMCd using the following commands:
    - $ cd C:\PalmDev\MMCd
    - $ make

5. Run MMCd
  - After successful build, click on launchInSimulator.bat to launch MMCd in the Palm Simulator.
  - If the Preferences dialog is displayed, click the home button and then launch MMCd.
  - To simulate a data connection change Options -> Preferences -> COM to Simulate in MMCd.

###KNOWN ISSUES:
- Buttons/checkbox controls can cause delays in log sampling rate
- mS timing uses a pretty awful hack  - but it works ;)
- EGRT Slugs don't immediately update
	
			
###INVESTIGATION:
- Min/Max control when monitoring (current Peak is always MAX)
- User defined prelog / postlog times (need to use Heap to conserve RAM)
- Consider adding default Autolog triggers (TPS > 80%)
- Consider adding default Alarm triggers (O2 / Knock)
- Correct slug for EGRT WBO2 AFR inputs - currently hard coded as MAP  - Fixed V1.8
- Need to confirm Boost solenoid status display
- Need to define/implement new log format (mS resolution / Integer / Long / Char )
- SmartScan to scan slowly changing sensors once/sec
- Consider adding Reset peak back to Monitor / Paused mode
- Consider adding 4 bar sensor used on some MAP gauges
	
###CHANGELOG:
9-4-05: V1.8c
- More tweaks to help system

9-4-05: V1.8b
- Fixed small bug in variable % scaling
- Fixed description for ISC - Idle speed control bug in sensor info list
	
9-4-05: V1.8a
- Changed to V1.8 to eliminate any confusion with parallel DSM effort 
- Added help info to Preferences and Alarm screens
- Remember to click on the "i" in the upper right hand of the dialog
- 4X slugs displays correctly on color and B&W Palms
	
9-4-05: V1.7K
- Fixed INJD Peak (too many hacks!)
- Changed to bold numbers in 4X display
- Removed units in 4X display to make more room 
- Added reverse slugs in 4X mode
- Fixed scaling error in RPM
- Changed AIRT / COOL Metric units back to xx.x C format 
- Changed % display format so < 100 displays xx.x % and > 100 displays XXX %
- Moved INJD to first bank and INJP to second page
- Misc mem trimming - project is REALLY out of space
- Appears that static VARS are not sharing code space
		
8-4-05: V1.7J
- Fixed AIRT Peak Polarity
- Fixed AIRT Alarm (Min/Max)
  - (AIRT already graphing correctly from earlier fix)
- Started shell of dynamic allocations to free up some memory
- Found that "Prefs" var was being declared but never used (waste of static space)
	
7-4-05: V1.7I
- Fixed INJ scan bug (if INJD was last sensor it would halt scanning)
- Brute force approach for now - will rip out INJD and provide scaling option in future release
- Added INJD logic to enabled RPM/INJP
- Modified 4X display logic so numeric displays can be configured while paused

6-4-05: V1.7Hcb
- Added non-linear O2 Graphing 
- Added O2 Non-Lin Preferences option
- Fixed menus
- Added back in degree symbols
- Out of memory error if EGRT slugs enabled
	
4-4-05: V1.7fg
- Added overclock ECU logic (MAF / RPM)
- Added overclock baud rate for direct connect
	
4-4-05: V1.7f
- Fixed missing break in MMCd.C that was screwing up num/graph toggle (hiding sensors)
- Found / fixed bugs in new baud / overclock code
- Weird string interaction in MMCd baseTime code (used str2 work-around)
- baseTime has to be UInt32 otherwise weird things happened
- Consolidated MMCd.C init code into setDefaults()
- Powerdown reset clears Peak buffer
- Disabled DashAlarm checkbox in alarm config
	
2-4-05: V1.7e
- Fixed Min/Max logic on Coolant Alarm setting (reversed)
- Fixed Min/Max logic on Coolant Peak / Hold (reversed)
- Switched large number diplay logic back to most recent always in upper left
- Peak automatically reset after returning from power-down
- Hide mode works in review mode - slick
- Added 4 pt space to right of 4X text
- Fixed peak logic in simulation
		
31-3-05: V1.7d
- Disabled peak hold on large digit (always displays current)
- Fixed peak on INJD related to earlier INJD logic change in V1.6p+
- Fixed INJD logic (finally)
- Added persistant storage for 4X config 
- Modified timing graph scaling (-10 before re-scaling)
- Changed power-down timing to 5 sec
- Cleaned up form display
- Reversed logic on big number selection 
- Review / Hide logic - sensor list is now filtered by available sensors in period
- Fixed form corruption when selecting Preferences - Default
- Checked timing calculation for accuracy
	
29-3-05: V1.7cb-4x
- Fixed form bugs
- Added timing graph scaling
- Added 4X numeric mode (repalces 1X jumbo text)
- Need to review other changes between 1.6P and 1.7aa

6-1-05: V1.7ad
- Save state bug fixed
- Eliminated implicit declaration warning

3-1-05: V1.7ac
- Created "Custom Sensors" menu under "Options"
- Moved "Custom Sensors" code from "Preferences" to "Custom Sensors" menu
- Implemented 3/S or DSM vehicle select in "Preferences" to scale timing values
- Implemented user defined base timing value for those with non stock base timing
- Removed 1920 and 9600 bps from the "COM" list
- Added 2148 and 2246 bps to the "Com" list to communicate with overclocked ECU's
- Moved "Simulate" to the top of the "Com" list
- Disabled prototype "New" menu for public release
- Changed default mode for a new log to Monitor_Mode
- Made all sensors selected for scanning by default
 
27-11-04: v1.7ab
- Fixed bug causing MMCd to crash when connected to the car, or in simulate mode
- Fixed "chunk over-locked" error
- Fixed resources locked error when exiting MMCd
- Fixed bug where INJD would display data when RPM and INJP were selected, even if INJD was not selected
- Added new menus for visual evaluation
- Added SmartScan enable checkbox in System Config
- Added Vehicle Select to select DSM or 3/S to scale timing value
- Added option to select your base timing to compensate for those running non stock base timings, allowing the true timing value to be logged

19-11-04: v1.7aa
- Restored broken functionality
- Moved Units list to System Config
- Added Log Rate, Display Rate to System Config
- Revised Sensor Config form
- Removed Threshold label and replaced with sensor slug
- Alarm can be set from old Alarms, or new Sensor Setup

17-11-04: v1.7a
- Graphical revision of menu structure for evaluation purposes
	
12-11-04: v1.6p
- Fixed coimpiler directive for new sensors
- Preferences dialogue COM: list box now 5 high (less scrolling)
- Fixed INJD bug in mmcd.c
	
15-07-04: v1.6o
- Investigating delay in serial rec/xmitroutine
- Added default AutoLog = true
- Added compiler directive to build with/without new sensor definitions
- Changed AFR scaling to 8.00 -> 22.0 (limited testing)
- Fixed overwrite of Reset button in monitoring mode
- Changed Speed: to COM: (solved display crunch)
			
01-06-04: v1.6N
- Changed max knock to 32 in graph scaling routine
- Fixed upside-down temperature graphs
- Fixed bug in postlog
- Added 56K high speed baud rate for expansion interface
- Testing new sensor definition/scaling
	
24-05-04: v1.6m
- Added structure to allow user graph scaling (no UI yet)
- Fixed graph bug that caused crash with simultaed data in graph routine
- Changed knock graph scaling to 0-25 knock count
- Added prelog of 20 samples (will add user config in future)
- Added Simulation tag in Speed preferences

20-5-04: v1.6l
- Fixed bug that caused crashes when switching from Review->Monitor
- Slight change in logic to enhance scan rates
- Added mS display resolution to Review display
- Added mS resolution to log for testing (using an unused sensor)
- Added new baud rates for expansion interface
- 1200 baud is actually simulation

14-5-04: v1.6k
- Fixed gap at start of log
- Enabled (B)oost and (F)uel relay in Flag 2
- Cleaned up Flag0 Flag2 mess in source code
- Disabled hard coded autolog criteria (TPS > 200)
  (Now user configured in Alarms)
		
09-5-04: v1.6j
- Changed postlog to 30 seconds
- Increased null data in graphs to 10 records
- Changed panel data to bold to enhance readability
- Reviewed / modified decimal formatting (significant digits)
- Known issues
  - Temperature inputs are upside down in graphs 
  - Min/Max reversed on temp inputs
  - Reset peak button overwritten if sampling > 10

02-5-04: v1.6i
- AUTOLOG FUNCTION
  - Enable / disable from preferences dialog
  - Autolog triggered by any user defined alarm condition 
  - Postlog currently set at 10 seconds (future release will allow user config)
  - Log control on front panel overrides autolog function
- Null data to identify discontinuities in log files
- Added min/max to large digit display	
- Known issues
  - Temperature inputs are upside down in graphs 
  - Min/Max reversed on temp inputs
  - Reset peak button overwritten if sampling > 10

29-4-04: v1.6h
- PEAK/HOLD
  - Peak mode enabled via new control on monitoring screen
  - Peak mode is enabled / disabled on the fly
  - Peak values are reset from new button on monitoring screen
  - Peak mode does not affect graphs
  - Large display is always live data 
-EGRT SCALING
  - Implemented 0-5V scaling
  - Implemented wideband O2 scaling (0-5V = 10-20 A:F ratio)

13-4-04: v1.6g - cb
- Added new O2 graph scaling 0-1.25V FS
	
17-3-04: v1.6f - cb
- Fixed bug in alarm code that was gating all alarms with TPS > 80%

17-3-04: v1.6f - cb
- Fixed bug in alarm code that was gating all alarms with TPS > 80%

29-11-03: v1.6e - cb
- Fixed row / column order sequeencing when in hidden mode
- Posted to board to get feedback

29-11-03: v1.6d - cb
- F1 key now sequences through sensors in proper row/column order
  when viewing large digit display 

28-11-03: v1.6c - cb
- Switches to PAUSE mode when engine is off to allow auto power off 
- Minor tweaks to menus
  - Rename / Upload Log
  - Upload log on next sync
- Fixed interesting bug in sensor scaling for negative PSI 

27-11-03: v1.6b - cb
- Added MAP sesnor confguration under preferences
- Supports 2.0, 2.5, 3.0 BAR (and scaling for original EGT)
- Open issues
  - Consider adding back EGRT tag if people were really using it
  - Logs do not include scaling info for sensors
    - (Alternative is to define separate MAP sensors tages /scaling)
  - Sensor scaling / graph scaling / color defined per sensor

26-11-03: v1.6a - cb
- Added MAP sensor capability
- Initially using 2.5 BAR MPX4250 sensor
- O2-R / O2-F alarms gated by TPS > 80% (Allows O2 < .93V alarm)


04-07-2003: v1.5a
- Added injector duty cycle calculation

03-07-2003: v1.4c
- If running on a Handspring device call HsKeyboardEnable(false) before
  trying to open serial port. Should work on Visors now.

01-07-2003: v1.4b
- Fixed a bug that caused graph scrolling to hang under Visor emulaton.
  It is still feels unresponsive, as event cueue is keeped cluttered by
  penMove events. Wonder if it is a case on real thing.

30-06-2003: v1.4a
- Splitted Edit form into Edit, Test and Debug.
- Moved Edit and Debug modes into they own forms accessible from main menu.
- Added high-density screen support. MMCd won't run on Tungstens though,
  because of limitations of serial port.
- Added double-density icons.
- Fixed a few double-density compatibility isues (cosmetic).
- Added MVIC button to test form.
- Minor conversion corrections for O2 and fuel trims.
- Replaced custom scrollbar with the stock one in Edit form.
- Splitted mmcd.c to test.c and debug.c.
- Moved resource ids definitions from mmcd.h to resource.h.
- Updated help for Preferences dialog.
- Updated Makefile.

20-06-2003: v1.3d
- Another bug in main form: now I was fooled by MemPtrResize. If there
  was more than 16 logs application crashed. Fixed.
- Added 'Backup' attribute indication to main form (dot in the end of
  log name).
- Added 'Backup on HotSync' checkbox to 'Rename Log' dialog.
- Permanently turned on Debug mode.

20-06-2003: v1.3c
- BUG: Main form code was broken, as it didn't take scrolling into
  account when opening existing log. as a result, it was possible
  to open only 11 first logs. Fixed.
- Turned out, that StrCompare() is using some non-obvious method of
  string compare and there's no plain strcmp() equivalent prior to
  PalmOS 4.0. Rewritten compare() in C.
- Inverted 'S' (P/S load present) bit in f_FLG2().
- Increased precision of conversion for FTxx, O2 voltage.
- In test mode renamed Faults to DTCs (for Diagnostic Trouble Codes).

17-06-2003: v1.3b
- Fixed a small bug with 'Hide' in Review mode: when log contains sample
  for custom sensor and this sensor is disabled in prefs, HideUnused()
  shown it anyway leaving a hole in bank.
- Updated air intake and coolant temperature conversion routines. Now with
  ones found in DSMlink source.
- Added O2-F sensor at 0x3e, renamed O2 to O2-R, deleted AIRV.
- Rearranged sensors.
- Some code cleanup.

16-06-2003: v1.3a
- Added 'Graph' checkbox to Monitor mode. When unchecked, graph is replaced
  by large numeric display of selected sensor. Button panel becomes
  single-selection.
- Added 'Alarms' menu. Allows to define alarm threshold and sound for
  each sensor.
- 'Graph' checkbox is controlled by hardware button 2
- In Monitor/numeric mode hardware button 1 cycles selection thru 
  sensors set for capture.
- In Review mode hardbuttons 1 and 2 move cursor left/right.
- PageUp/PageDown hardware buttons change current bank.
- Hardware button are blocked 1..4 in Edit form whenever unused.
- State of 'H' button, numeric mode and selection is saved on exit.
- Added PalmOS version check (>= 3.5) at startup.
- Removed 'Compatible graphcis' from prefs as it appears unuseful.
- Added 'Alarms' checkbox to prefs.
- Fixed a bug in barometric pressure conversion routine.

11-06-2003: v1.2
- Fixed (I hope) timing advance conversion.
- Added English-mode pressure conversion (in psi).
- in Monitor mode 'Log' and 'Pause' checkboxen can be activated by hardware
  buttons 3 and 4

06-06-2003: v1.1
- Applied patches submitted by Shawn Pulson <spoulson@rcn.com>
- Reduced timeout in EvtGetEvent() as 1 second timeout breaks graph
  scrolling code.
- Replaced lookup table by XPOSE() macro
- Moved bank selection buttons 1 pixel left off 'H' to make 1-pixel
  gap between borders as it looks more pleasant to me :)
- Bumped version to 1.1

03-06-2003: v1.0d-shawn-2.1
- Update user interface
- Update display of sensors in Hide Unused mode, so that they are
  displayed correctly in column orientation.  As put by Dmitry:
  1 x     1 x   and not... 1 x     1 x
  x x     2 x              x x     2 x
  2 4     3 x              2 4     4 x
  3 x     4 x              3 x    3 x
- Update optimization wait in main loop by instead using the commented
  code block for EvtGetEvent(...).  This is cleaner and doesn't slow
  down the GUI by sleeping.

01-06-2003: v1.0d-shawn-2.0
- Update user interface
- Instead of 5th 'X' bank, use 'H' pushbutton in upper right to
  toggle "Hide Unused" mode.
- In Review mode, it will dynamically show only sensors logged at
  the selected graph point.
- In Monitor mode, it will show only sensors captured or graphed.

30-05-2003: v1.0d-shawn-1.0
- Update user inteface
- Add a 5th 'X' bank that, when selected, will show up to 8 sensors
  captured or graphed.
- Add optimization wait in the main loop to keep cpu utilization low.

23-04-2003: v1.0d
- Added "Compatible graphics" checkbox to prefs dialog. Looks like
  direct access to bitmap bits doesn't work on Sony CLIE...

16-04-2003: v1.0c
- Fixed a bug with prefs. When running for the first time default
  preferences get trashed. Added "Default" button to prefs dialog.
  (thanks to Steven_GT)
- Removed call to BmpGetBitsDepth unavalable prior to PalmOS 4.
  (thanks to Jeff Oberholtzer)
- try to open serPortCradlePort if serPortCradleRS232Port fails.

31-03-2003: v1.0b
- Implemented baud rate and measure system in prefs dialog
- Fixed Purge command in Test form. Pressure, EGR and Boost need testing.

28-03-2003: 
- Added scrollbar to main form (it still needs some work, though)
- Implemented test mode. Injectors, Fuel pump, Fault erase should work
  as expected. Fault codes are not checked throughly - some assistance
  needed. Others may work, may not, may be permutated
- Removed Debug mode
- Added prefs dialog

27-01-2003: v1.0a
- Main form doesn't scroll, so up to 11 logs can be selected
- Test mode not implemented yet and contains debugging stuff
- Settings dialog not implemented
