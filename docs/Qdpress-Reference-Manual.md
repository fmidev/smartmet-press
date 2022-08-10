Copied as-is from ancient Word-files. Feel free to improve.

Comments, includes and directives can be used in both the qdpress and manager files. They all, as well as variable substitutions, are performed during preprocessing.

***

`#!PressProduct`

Required first line in every qdpress definition (.pre file). Case sensitive. This command also initiates the product object (main level). Other objects (# initiated) are  members of the product object or successively members of these objects.

## Comments

Block comments spanning several lines can be used as well as line comments.

`/* */`

Block comment. Nesting may cause issues.

`//`

Line comment

`#Include [path]filename[.suffix]s`

File to be included on this place. These files may be nested.

## Variables

Define variables in `season_status.txt` file. Variable defining lines start
with text `Variable`.

The character sequences beginning `$#` denotes global variables which are replaced by their respective values during preprocessing. The variable names may be 1-n characters long. They may be used almost anywhere as key words or attributes or part of them without delimiters, but their use in Manager is not possible.

Format: `Variable $#Name value`

A variable may not be a substring of another. For example only either of `$#r` or `$#rain` is allowed, but not both. Although it may work if processed in proper order and you are feeling lucky.

Variable names are case sensitive. The variable values shall be defined in the season_status.txt file.

Example would output text `heavysnow` when value is between 1.5 and 99.

```text
season_status.txt:
// Threshold for heavy snow
Variable $#s 1.5

some_product.pre:
Mapping = $#s 99 heavysnow
```

## Conditional Directives

Directives are denoted with prefix `#`, but also some other commands are (starting of objects). Directives are mostly case sensitive. Special attention should be put to correct directive syntax because qdpress's ability to recover from errors is restricted.

Conditionals start with `#ifCondition...` and end with `#condition...Endif`
Be careful to write the corresponding `Endif` directive for every started condition.

```text
#ifConditionDate<DD1><MM1>-<DD2><MM2>
   <text block1>
[ #conditionDate<DD1><MM1>-<DD2><MM2>Else
   <text block2>]
#conditionDate<DD1><MM1>-<DD2><MM2>Endif
   <DD> = 01|02|………30|31 (local time)
   <MM> = 01|02|………11|12 (local time)
````

The preprocessor includes `block1` if the date is inside the period `<DD1><MM1>` and `<DD2><MM2>`, and `block2` if not. The period may pass the year turn.

Example would produce value `Yeah` from end of October untill beginning of May and suring summer and autumn value `Baby`.

```text
#ifConditionDate3011-0502
   Text = "Yeah"
#conditionDate3011-0502Else
   Text = "Baby"
#contionDate3011-0502Endif
```

```text
#ifConditionDay<Ww>…
    <text block1>
[ #<Ww>…Else
    <text block2>]
#<Ww>…Endif
  <Ww>= Mo | Tu | We | Th | Fr | Sa | Su
```

Allows conditional inclusion of definition blocks due to any of 1-7 days of week. The days can be in free order but the `<Ww>`... strings must be identical. The preprocessor includes block1 if it is run during any of the days of week, and block2 if not. The day of week to obey can be changed from the true one to another in the season_status.txt file (for tests or exceptions). The `<Ww>`... strings may not be a substring of another `<Ww>`... string. I.e. you may not use conditions MoTh and MoThSa in the same definition (but MoTh and MoSaTh can be used).

Example:

```text
#ifConditionDayMoWeSa
    Text = Snowmap
#MoWeSaElse
    Text = Precipitation
#MoWeSaEndif
```

```text
#ifConditionHour<hour>
   <text block1>
[ #conditionHour<hour>Else
   <text block2>]
#conditionHour<hour>Endif
   <hour> = 0|1|2………22|23 (local time)
```

The preprocessor includes block1 if `<hour>` has been reached at run time, and block2 if not. Note that local time is used, not utc. One digit numbers can be written with or without leading zero. The `<hour>` to obey can be changed from true time to another time in the season_status.txt file (tests or exceptions).
Example: #ifConditionHour5 #TimeText  Size = 8  Place = 150 200  #ConditionHour5Endif

```text
#ifPollenOrSnowPeriod
    <text block1>
[ #pollenOrSnowPeriodElse
    <text block2>]
#pollenOrSnowPeriodEndif
````

PollenPeriod and SnowPeriod are defined inside the program and PollenOrSnowPeriod is true when one of them is true. The preprocessor includes block1 if we are inside the PollenOrSnowPeriod and block2 in the opposite case. Actual status of both PollenPeriod and SnowPeriod can be set in the season_status.txt file.

```text
#ifPollenPeriod | #ifNotPollenPeriod
    <text block1>
[ #pollenPeriodElse
    <text block2> ]
#pollenPeriodEndif
```

PollenPeriod is at FMI used to indicate the time, during which there are enough pollen in the air to cause allergies (special symbols can be used in the illustration according to Pollen status). PollenPeriod is defined inside the program. In the present version it is set to 31.3. - 31.8. The preprocessor includes block1 if we are inside the PollenPeriod ( | outside it) and block2 in the opposite case. Actual status of PollenPeriod can be set in the season_status.txt file.

```text
#ifSnowPeriod | #ifNotSnowPeriod
    <text block1>
[ #snowPeriodElse
    <text block2>]
#snowPeriodEndif
````

SnowPeriod is a calendar period defined inside the program. In the present version it is 15.10. - 10.5. The preprocessor includes block1 if we are inside the SnowPeriod ( | outside it) and block2 in the opposite case. Actual status of SnowPeriod can be set in the season_status.txt file. Usually this is the normal, because it's convenient to start the period when the snow falls and stop it when it melts.

```text
#ifSummer | #ifNotSummer
    <text block1>
[ #summerElse
    <text block2> ]
#summerEndif
````

Summer is a calendar period defined inside the program. It is set to the months June – August. Exactly the period is 31.5. - 31.8 because products typically are made starting from the next day. The preprocessor includes block1 if it is Summer (| not summer) and the block2 in the opposite case. Actual status of Summer can be set in the season_status file (e.g. for tests).

```text
#if<weekday>
    <text block1>
[ #<weekday>Else
    <text block2> ]
#<weekday>Endif
  <weekday>= Monday | Tuesday | Wednesday | Thursday | Friday | Saturday | Sunday
````

The preprocessor includes block1 if it is run on the `<weekday>`, and block2 if not. The day of week to obey can be changed from the true one to another one in the season_status.txt file (for tests or exceptions).

```text
#ifWinterTime | #ifNotWinterTime
    <text block1>
[ #else
    <text block2> ]
#endif
```

WinterTime=standard time as opposite to summer time=daylight saving time. The preprocessor includes block1 from the definition file when it is WinterTime ( | not WinterTime) and block2 in the opposite case. True status of winter time can be changed in the season_status.txt file (e.g. for tests).

```text
#Weekday OnlyIn  <period>
   <period>= Summer | SummerTime | WinterTime | SnowPeriod | PollenPeriod  (not case sens.)
````

This offers a way to totally exclude day-of-week dependent blocks according to various seasons. The parts inside the `#if<weekday>` directives are included only during the `<period>`. Note that the weekday blocks are left out (when outside the `<period>`) regardless if status of them is true or false. SummerTime here refers to !Wintertime in the season_status file. Note that there are three separate words.

Example: #Weekday OnlyIn WinterTime

```text
#Weekday OnlyOutside>  <period>
<period>= Summer | SummerTime | WinterTime | SnowPeriod | PollenPeriod  (not case sens.)
````

This offers a way to totally exclude day-of-week dependent blocks according to various seasons. The parts inside the `#if<weekday>` directives are included only outside the `<period>`.  Note that the weekday blocks are left out regardless of status of themselves. SummerTime here refers to !Wintertime in the season_status file. Note that there are three separate words.
Example: #Weekday OnlyOutside Summer

## Ordinary Commands in qdpress

These commands are not allowed in the Manager unless defined there also (see appr. section). Ordinary statements (objects, attributes and their settings) are normally not case sensitive. However, e.g. file names, are. The defaults for attribute settings are shown on command lines when suitable. The possible containing object(s) are shown under the command. First are shown the object (starting) commands (prefix #) and after them other commands. Both categories are listed in alphabetical order.

`#ComputerTime`

Valid for: `#Product`, `#Segment`

Object showing wall clock  time. All time outputs are by default in local time but can also be defined to utc. The time string is constructed according to the given format (default “Ww H”) and all normal text attributes are available. The only required is the coordinate point where to put the string. Other attributes have default values or are inherited from higher levels.

`#ExtremePlace`

Valid for: `#Parameters`

The location name for the scanned extreme value. Used in connection with #Number+option AreaMinimum or alike (not e.g. with Minimum). Normal text attributes available.

`#FixedSymbol`

Valid for: #Product, #Segment, #Parameters, #FixedSymbolLocations

A static (not data dependent) symbol. Consequently no data is needed.  Adequate for icon type images. The image should be found in the symbols/”SymbolSet” (see command) directory given.  If defined on #Product level one icon will be achieved, if defined on #Segment level an image for each location is produced and if defined on #Parameters level one image for each location * each_time is produced. See also #FixedSymbolLocations.

`#FixedSymbolLocations`

Valid for: #Product

A list of  locations for a #FixedSymbol object. Convenient if an identical static icon is to be placed on more than one point in the illustration.

`#Letter`

Valid for: `#Parameters`

Text string derived from (weather) data. The string is derived from the data according to location and time of the #Segment and the parameter(s) of the object defined according to the mapping directives. Normal text attributes are available, and are inherited from higher levels.  Also various computational options can be applied on the data. The text string is repeated according to the possible time and/or location loops of the #Segment.

`#Map` and `#MapEnd`

Valid for: #Product, #Segment

Geographical map object. Longitude/latitude coordinates are mapped to illustration (x,y) coordinates, as well as the other way, by this object. A #Product level #Map object can be overridden in the #Segments.

Closing command for the geographical `#Map` object is `#MapEnd`. #Map is the only object, which must be closed explicitly (technical reasons)

`#NameDay`

Valid for: #Product, #Segment

Name day string. There should be a name file (namedays_xxx.txt) containing the name days for the used language. The file format is a dedicated one for qdpress. Normal text attributes are available. The string may be limited to a number of names or alternatively to the number of characters. When used in #Segment you can take advantage of the time loop option.

`#Number`

Valid for: #Parameters

Presents (weather) data as numbers in the illustration. The number is derived from the data according to location, time and parameter. Default parameter is temperature. All normal text attributes are available as well as many options for number presentations. Also various computational options, like min/max/sum, randomization, conversions (Fahrenheit, windchill, degreedays) etc can be applied on the data. The number is repeated according to the possible time and/or location loops of the #Segment.

`#Parameters`

Valid for: #Segment

A container object for data presenting objects (temperatures, weather symbols, winds etc). The objects grouped within the #Parameters will step through the #Segment’s all time steps and all locations. Some object attributes (colors, text attributes) can be defined jointly on this level.

`#RotatingSymbol`

Valid for: #Parameters

As #Symbol but the icon can be rotated according to a second parameter. Used for wind symbols. No need for any other rotating parameters has appeared, so rotating is fixed to the values 0 – 360 for one full rotation. The symbol is stored showing the value 90 (wind from west). The symbol is repeated according to the time and location loops of the #Segment.

`#ScaledSymbol`

Valid for: #Parameters

As #Symbol but the image can be scaled according to the value of another parameter. Scaling can be performed differently in x and y direction.

Maybe handy for example resizing wind arrows.

`#Segment`

Valid for: #Product

A `#Segment` is primarily a list of geographical locations to be handled identically. Often there is a time list or loop, which all locations go through. The overall structure therefore is a two dimensional time / location table.  A third dimension comes with the weather parameters, listed as separate objects within the #Parameters object. The parameter values are read from the data file defined on the #Segment level or as default the first data listed on #Product level.

Various member objects can be defined on #Segment level. They are performed in relation to the loops as appropriate, i.e. #TimeText shows the time for every time step (not every location) and #StationName for every location (not every time). Whereas inside the #Parameters every member object is performed for every time step and every location.

`#StationName`

Valid for: #Segment

Object for showing location names. All locations in the #Segment’s station list are presented by name. Names are written during the first time step in a time loop. Hence, (x,y) coordinates shall be related to the initial time step. The names are the ones used as station identifier for the data in the definition. A substitute can, however, be given in the definition code (see StationNewNaming (language changes, abbreviations etc)). Regular text attributes are available. Instead of using #StationName the names could be pre-written on the background, but it is strongly recommended to use #StationNames in order to minimize conflicts and to simplify modifications.

`#StationNumber`

Valid for: #Segment

Object for showing data location wmo-numbers. As #StationName but for WMO numbers. Seldom needed.  Applies only for point data.

`#SubComputerTime`

Valid for: #Text #TimeText

A “wall clock time” string to be added to the string of the containing object. The attributes cannot be changed and own coordinates are not applicable because this string is just concatenated. The time and its format can be defined.

`#SubImage[s]`

Valid for: #Product

Including an eps image into the product. The image must be in EPS Version 8 format. Contrary to the BackGround command there are many options which you can be used to place and rescale the image with. Additional images may be called inside a #SubImage, thus using partly the same attributes (see “NewName”, “NewFileMoving”)

`#Substitute`

Valid for: #Parameters

Enables the use of date dependent parameter values in (multi)mapping. You may want to produce the text “very cold” round the year.  You should not put a universal limit, say -25, as the condition, because in July you want the text to be generated starting below +10 degrees.

With proper data available you can have the limit set at run time inside the program. In the definition code, you use a unique value in a mapping rule (should now be in range 31690-31699). That value shall be connected to a parameter in a previous #Segment. The actual limit is then picked up from the data according to date and geographical location. Unfortunately this technique can be used for only one geographical location at the same time (but for nearby locations the difference is not be very large).

Example:

```text
#Segment
    Station = Helsinki  10  10
    Data = Norm71_00
    #Parameters
        #Substitute
            Parameter = 812 // 2.5% fractile
            MappingValueToSubstitute = 31690
        #Substitute
            Parameter = 813   // 12.5% fractile
            MappingValueToSubstitute = 31691
#Segment
    Station = Helsinki 213  450
    #Parameters
        #Text
            // cloud./prec.mode/thunder%/intens./temp.
            MultiParameters = 79   57   260   353      4
            MultiMapping = -, -, 0 29, 0 0.01, -99 31690  very_cold
            MultiMapping = -, -, 0 29, 0 0.01, -99 31691  cold
````

`#SubText`

Valid for: #Text #TimeText

A string to be added to the string of the containing object. The attributes cannot be changed and own coordinates are not applicable because this string is just concatenated. The string may be given directly or read from a file.

`#SubTimeText`

Valid for: #Text #TimeText

A time string to be added to the string of the containing object. The attributes cannot be changed and own coordinates are not applicable because this string is just concatenated. The time and its format can be defined.

`#SunTime`

Valid for: #Parameters

The time for sunrise or sunset (specified which one in the object definition). The location(s) and time(s) are the actual ones for the #Segment during the optional time and/or station loops.

`#Symbol`

Valid for: #Parameters

Showing of (weather) symbol(s) in the illustration. The symbol is derived from the data according to location, time and parameter(s). The symbol is repeated according to the possible time and/or location loops in the #Segment.

The symbols must be stored in Windows Illustrator version 8 format in named directories (`SymbolSet`).

When using a particular symbol for the first time, the application stores a truncated copy of the symbol file (size approx. 20% of original), which is used in future runs. This must explicitly be deleted when the symbol is updated.

Also the symbols used from subdirectories need to have a similar directory created in the symbol cache.

`#Text`

Valid for: #Product, #Segment, #Parameters

Text string object. The only mandatory attributes are the string itself and the coordinates to place it on. The string can be given either directly (short, static), or in a file (dynamic).

Texts are either placed according to a single point or inside a coordinate path. Various attributes are available having reasonable default values or are inherited from higher definition levels.

Inside the #Parameters object, attributes are not inherited from containing objects. The string is repeated according to the possible time and/or location loops of the #Segment.

Note that the string is static also inside #Parameters, not varying according to the data as other objects there.

Use `#Letter` for data dependent strings.

`#Time[Text]`

Valid for: #Product, #Segment, #Parameters

Time notation. The time to be written is the actual time of the containing object. All time outputs are in local time unless defined to be utc. The time string is constructed according to the given format (default “Ww H”) and all normal text attributes are available.

The only required attribute is the point where to put the string on. Other attributes have default values and are inherited from higher levels.

In #Product: one time notation is printed

In #Segment: a time for every time step is printed.

In: #Parameters a time for every time step and every location is printed (number = timesteps * locations).

`#DayChangeText`

Valid for: #Segment

Not yet implemented!!!

String planned to be used when the day is changed to yesterday, because the time is too early for the observation from that particular station.

Used in connection with option “UseBackupPreviousDay”. However, the use of it has not (yet) been implemented, although defining of it will succeed.  For the time being an asterisk is shown after the station name. But with this object you could have any text placed anywhere.)

`#WindDirection`

Valid for: #Parameters

As #RotatingSymbol but the  attributes “Parameter” and “RotatingParameter” are by default set to proper parameter numbers, so you can save two definition lines (and have self documented code). The wind direction is repeated according to the time and location loops in the #Segment

`#WindSpeed`

Valid for: #Parameters

As #Number but the parameter is by default wind speed. Documents the definition code better and saves one line. The wind speed is repeated according to the time and location loops of the #Segment.

***

`AcceptanceInterval = fromValuef  toValuef`

Valid for: #Parameters members

Only values between fromValue and toValue will be accepted, others are missing (i.e. shown normally with “-“). If fromValue is bigger than toValue the function is the reverse.

Examples:

```text
AcceptanceInterval = 0 10 -> values between 0 and 10 are accepted
AcceptanceInterval = 10 0 -> values between 0 and 10 are rejected
````

Note that “TextMapping = 0 10 None” will not even show “-“ when come true.

`AddAfter = strings`

Valid for: GenericTexts, GenericParamTexts

Adds the string after the resulting text.

`AddInFront = strings`

Valid for: GenericTexts, GenericParamTexts

Adds the string in front of the resulting text.

`AdditionalSymbolScale = additionalScalef`

Valid for: #Product

Legacy. Please do not use. Rather make sure all symbols in all directories are matching size.

This command will make it easier to change the used symbolset globally. If you have been using a specific symbolset and change it to another, normally you also must change the “Scale” commands in all #Symbols, because the nominal sizes probably are different. “AdditionalScale” gives the scaling from the former symbol size to the new one. The sizes will remain and also the proportions between symbols in different objects.

Example

A symbol has nominally been 40 points high and you have used it 20 points high setting Scale = 0.5. The symbol in the new set is nominally 10 points. You can specify “AdditionalSymbolScale = 4” instead of correcting in every #Symbol “Scale = 2”.

`AddlocalTime``

Valid for: #StationName

Adds the local time in parenthesis after the station name. The embedded algorithm is based on wmo-numbers which must be found in the data or the Name2LongLat file. Has been used in observation tables from around the world when a fixed utc time is presented:

* Tallinn (14)+3
* Beijing (20)+8
* New York (07)+7

`AddStationName`

Valid for: #Number

Adds the location name to the result value.

`AllowMissing`

Valid for: `#Parameters` members

Allows especially minimum, maximum and alike processing to give a result, although it may not be absolutely correct (because data from some locations may be missing). Without AllowMissing the result would be “missing”.

`Alignment = Center | Left | RightCenter`

Valid for: #Product, #Segment, #Parameters, GenericTexts, GenericParamTexts

The alignment for point-placed (as opposite to path-placed) text strings. Horisontally they can be aligned to the left, center or right. Vertically they are always aligned to the center point (approx.).

Alignment can be set as global attribute for lower levels e.g. in the #Product  and #Segment object  as well as in the text objects themelves. The alignment is inherited to possible member objects and can always be reset  there.

Examples of typical alignments:

* Center for temperatures on map
* Left for city names in table
* Right for temperatures in table

`AnalysingTime`

Valid for: #TimeText

The time notation give the analyzing time of the actual data file. “AnalysingTime” is valid only inside a #Segment, because outside any actual data isn’t relevant.

`AreaMaximum`

Valid for: #Parameters members

The result is the maximum of all data locations for the actual time. If `AllowMissing` is declared the value is calculated despite locations with missing values.

`AreaMean`

Valid for: ParametersMember

The result is the mean value of all data locations for the actual time. If AllowMissing is declared the value is calculated despite locations with missing values.

`AreaMinimum`

Valid for: ParametersMember

The result is the minimum of all data locations for the actual time. If AllowMissing is declared the value is calculated despite locations with missing values.

`AreaOperation`

Valid for: #Segment

The operation is dealt with a geographical area rather than with (a) single point(s). The area is defined in a #Map object. All data points inside the rectangle given in the #Map are processed. Areal operations may be “AreaMean”, “AreaMaximum”, “AreaMInimum” or “AreaSum”, which is defined in the member object of #Parameters.

`AreaSum`

Valid for: ParametersMember

The result is the sum of all data locations for the actual time. If AllowMissing is declared the value is calculated despite locations with missing values.

`AreaPath = [xf yf ...]`

Valid for: GenericTexts

An arbitrary shaped path, inside which a  text is to be written. At least three points required. If the path forms a rectangular area it’s more simple to use ParagraphBorders. If the whole text does not fit into the area its is cut off. Alternative to Place and ParagraphBorders.

`AvoidOrphanSyllables`

Valid for: #Text

Avoids leaving single syllables on separate lines when wrapping texts in columns. As default there are single syllables (not yet single characters).

`Background = [path/]backgroundfile`

Valid for: #Product

A file to be used as a background illustration. Any other produced information will come in front of it.

The file must be in Windows Illustrator 8.0 eps-format. Several background files can be used and are placed in the illustration in the order they are listed. A background file is placed as originally stored. If replacing or scaling is needed use #SubImage instead.

`BackupStation`

Valid for: #Segment

The next `Station...` command is a backup location for the previous `Station...`, in case the data would be  missing.

`BoundingBox = left bottom right top`

Valid for: #Product

Size of the product in Post Script points. The bounding box borders for the illustration set in the eps-file. E.g. pdf-files are clipped according to these borders.

`BypassValue =  valuefParametersMember`

A parameter value to completely bypass. (The what in the what what? -ed.)

`Capitalize`

Valid for: GenericTexts, GenericParamTexts

The first character of a string will be upper case. Maybe nice for station definitions.

`CharSpaceJustification =  valuef`

Valid for: GenericTexts, GenericParamTexts

The character spacing is adjusted by “value”. Enables fitting a string to a specific width. Negative values will narrow the spaces and positive will broaden them. E.g. +2 gives approx. double size spaces and -1 almost none spaces. It is accomplished in the PS code by the operator ashow.

`ClippingPath =  [xf   yf]…`

Valid for: #SubImage

Crop included SubImage to defined path. The eps-illustration is clipped according to this path. At least three points are needed to build an area.

`Clip[pingRectangle] =  leftf  bottomf  rightf  topf`

Valid for: #SubImage

The eps-illustration is clipped according to these borders.

`CMYKColor  =  cf  mf  yf  kf`

CMYK  0 0 0 1(black)

Valid for: #Product, #Segment, #Parameters, GenericTexts, GenericParamTexts

CMYK color setting for objects which obey color setting (texts). The components are in the range 0–1. Default color space is CMYK. CMYK should be used for print media. Colors can be overridden on lower levels.

`CMYKMapping = lowLimitf highLimitf cf mf yf kf`

Valid for: GenericParamTexts

Set color with parameters values. Values in the range “lowLimit” to “highLimit” are presented in the defined color.

`CoordinatesFromMainMap`

Valid for: #Segment

The (x,y) location coordinates in this #Segment refer to another illustration (a former product). This map can and must be defined on the main level. In this #Segment you have the map for the actual (new) layout.

A conversion is performed from the former map to the new one. This is a fast way to use location lists from former products without changing all (x,y) coordinates. Future modification will, however, be more difficult, because you have to design in terms of the original illustration.

`CurrentScalingPoints = leftf  bottomf  rightf  topf`

Valid for: #Product, #Segment, #SubImage

When rescaling is used, this command gives the new (current) area. The command is used in cooperation with InitialScalingPoints. Together they define the rescaling from the initial to the current illustration.

Scaling is independent in the two directions and offset can be replaced. Useful e.g. when a product get a new background map and the set of locations still would fit the new map as geographical places. The command pair can also be used when a fixed location set has been designed for standardized use on various backgrounds.

In #Subimage the InitialScalingPoints & CurrentScalingPoints pair is used to place the the image correctly. E.g. contur fields can be placed and scaled on maps according to map backgrounds.

`CurrentScalingAndClipping =  leftf  bottomf  rightf  topf`

Valid for: #SubImage

The commands “CurrentScalingPoints” and “Clip” by one command when yoy can use the same coordinates. If the image is scaled by the corner points it is often possible to use this option.

`Data = [path]datafilenames`

Valid for: #Product, #Segment

Data file to be used. Data must be in querydata format and can either be ascii or binary coded (fqd/sqd).  All used data files in the entire product must be declared on main level. It is recommended to put them in the first lines, because they have impacts on some other commands. The first data file mentioned is also the default for all #Segments. In the #Segments you normally only deal with one data file per #Segment, but you may also take advantage of another file by the command PrimaryDataFile.

`DataFilesCritical`

Valid for: #Product

The program will stop if a data file, which has been defined,  not is found. The interpretation of this command is, however, sequential (contrary to most commands). It applies only to the files coming after the command, until the influence possibly is interrupted by the command “DataFilesNotCritical”.

`DataFilesNotCritical`

Valid for: #Product

Data file definitions coming after this command are not considered critical (requiring interruption of program). The command DataFilesCritical has presumable been given before this, because the default is that they are not critical. See “DataFilesCritical”.

`DataNotNeeded`

Valid for: #Segment

In some occasions it may be worthwhile to use this option to avoid pointless warnings etc. when a data file really not is needed.

`DataTimestamp = name[.suffix]s no timestamp`

Valid for: #Product

Name of the output file including timestamp. Default format is nameYYYYMMDDHHmm, but the timeformat can be specified in the ProductNameTimeFormat command. The time is the main level (#Product)  local time. In Finland often tomorrow at 1400 o’clock (summer time 15).

`Day = dayOffset + 1`

Valid for: All applicable

Offset of time in days for starting time of object. The default has been chosen to +1 because most of the newspaper weather illustrations start from day +1. Inherited to member objects, and are there often stepped forward.

`DayAddition = additionalOffseti +1`

Valid for: #Segment

An additional offset to the main (#product) level time offset in days.

`DegreeDays`

Valid for: #Parameters members

Value is presented as degree days. Following FMI definition.

`DeleteScandinavianFromProductName`

Valid for: #Product

The Scandinavian characters “åäö” are replaced with “aao” in the output file name, e.g. when the name is derived from a city name.

`DetachSign`

Valid for: #Number

An extra space is placed between the (integer) value and the sign in one digit numbers, but not in longer strings. The option has been done in order to optimally fill spaces reserved for 3 digits.

`Directory = directoryf`

Valid for: #SubImage, #Text

Directory for a file given separately. On Windows the directory shall be on the level below the applications head directory. Also subdirectories may be given. Normally you give also the directory in the “File” command.

`DistanceCheck  deltaXf  deltaYf`

Valid for: #Segment

Prevents objects in the same #Parameters-object (numbers, symbols etc) to be drawn inside the same rectangular area (deltaXf  , deltaYf ). The first one in the location list is favoured, unless the parameter is pressure. Then the more significant one is shown. The significance is defined as the difference to the value 1015 (i.e. lows are slightly preferred before highs). If the pressure is below 995 the allowed distance is reduced from the given one (nearby low pressure subcenters). “DistanceCheck” have been used to prioritize Low/High centers.

If coordinates are rescaled in the definition (Initial../CurrentScalingPoints) the “DistanceCheck” is operating with the initial coordinates.

`DotToComma`

Valid for: #Number

Decimal dot is printed as comma, e.g. 3.5 -> 3,5.

`Encoding = encoding`

Valid for: #Product

Encoding used for output. Global setting. Default is `latin`, everything else...?

`EquiDistanceMarking = roundingConstantf  approvingRangef`

Valid for: GenericParams

The value is rounded to the nearest number divisable by roundingConstant. Only values within the range +- approvingRange are approved. For contur labelling the command EquiDistanceAndAdjusting is recommended instead of this.

Example:

```text
EquiDistastanceMarking = 5  0.6
  10.4 -> 10
  10.7 -> rejected
````

`Extremes = maxStrings  minStrings`

Valid for: #Letter

The data points in a grid field are checked, whether they are local extreme values. A local maximum will be marked with “maxString” and a local minimum with “minString”. The locations are adjusted to exact coordinates by using gradients. You can set the minimum distance between flagged extreme values by the command “DistanceCheck” on #Parameter level. This command will function properly only for pressure values. With “DistanceCheck” given, of two values too close to each other, the more extreme one is chosen.  If a low and a high are too close, the one more distant from the value 1015 is chosen. Values below 995 are treated more lightly (storm subcentres). The command “UseGridAsStationPoints” shall be given in order to have all grid points processed.

Examples:

```text
Extremes = H L
Extremes = High Low
````

`EquiDistanceAndAdjusting = roundingConstantf  affectengDistancef`

Valid for: #Letter,  (other GenericParams)

The value is rounded to the nearest number divisable by roundingConstant. The rounded value shall be found within a radius of approx. affectingDistance from the given coordinates (usually all points of a grid), otherwise it is rejected. The place of the number is adjusted, trying to find the exact place by using gradients of the field. The smaller the affectingDistance is, the better the values will represent exact values, but at the same time there will be fewer of them altogether. If conturs are labeled the command EquiDistastanceAndAdjusting preferred before EquiDistastanceMarking.

Example:

```text
EquiDistastanceAndAdjusting = 5  12
```

`Fahrenheit`

Valid for: #Number

Temperature in Fahrenheit.

`File = [path/]files`

Valid for: #Text

The file containing the text to be written. Especially texts which can change every time, are given in a file. Alternative to attribute Text.

`FileWithTimestamp = [path/]files`

Valid for: #Text

As “File” but the file name shall start with a timestamp in format NNDD, which must match with the actual date of the #Text object. The file containing the text to be written. Especially texts which can change every time, are given in a file. Alternative to attribute Text.

`FillWithUnderscore`

Valid for: #ExtremePlace

The resulting name string is filled with underscore up to defined ”MaxLength”.

Example of output:

```text
Helsinki____________________
Riga_______________________
```

`FinnishTimeZone`

Valid for: #Suntime, #TimeText, Parameters

Finnish time zone is applied (= Latvian).

`Font = fontnamesHelvetica`

Valid for: #Product, #Segment, #Parameters, GenericTexts, GenericParamTexts

Actual font for strings. Case sensitive. This is just a name put into the output file and no error checking can be performed. The font shall be available on the computer illustrating or converting the file. The font is inherited to member text objects.

`Format = timeFormats`

(Note that there is another use of Format for numbers)

Valid for: TimeTexts

The format for the time string. For details see “TimeFormat” at the end of the Syntax Manual.

Example:

```text
Format = “done D.M.YY at HH:mm”    ->
done 8.2.12 at 09:23
````

`Format = numberFormats``

(Note that there is another use of Format for time)

Valid for: #Number

The format for the number string. Follows C language standards with a few extensions. For details see “NumberFormat” at the end of the Syntax Manual.

Example:

```text
Format = %+.1f    ->
+12.3
````

`FromMeanWindToMax`

Valid for: #Parameters

A max wind speed will be derived from the mean wind. No parameter identifiers are required.  The formula is very simple:
maxWind = (max(meanWind * 1.25, 2))

`HeaderFont = fontNames`

Valid for: #Text

Font for the header (bolded) part of a text. The HTML tag `<b>` is used to mark the header in the input text. This enables the use of one single text file including both headers and body text. See also MainHeaderFont.

`HeaderSize = sizef`

Valid for: #Text

Font size (in points) for the header (bolded) part of a text. The HTML tag `<b>` is used to mark the header in the input text. This enables the use of one single text file including both headers and body text.

`HeightScale = zeroValuef  nominalValuef  maxValuef`

Valid for: #ScaledSymbol

The scale formulae to be applied to a #ScaledSymbol in y-direction. “NominalValue” denotes the stored size, zeroValue the value for which it goes to zero height, and maxValue the highest allowed data value. The scaled size is achieved according to: (max(value,maxValue)-nominalValue)/(nominal¬Value-zeroValue). Can be used e.g for bar charts. Then the original symbol shall be stored with the base at zero-y in order to have the base fixed for all sizes. See “ScalingParam” for the data param controlling the scaling.

Example:

```text
HeightScale = 0 2 20  ->
````

e.g. value 10 gives a symbol five times the original height.

`Hour = utcHour`

Default: 12

Valid for: #Product, #Segment,  GenericParams

Actual hour of day for the object, i.e. the time of the data to be presented. It is defined in utc. Hour is the shortest time unit in qdpress, minutes cannot be used. The default has been chosen to 12 because most of the newspaper weather colums start from tomorrow midday (local time shifted one hour summer/winter). Inherited to member objects.

`HoursFromData  = hoursi`

Valid for: #Product, #Segment

The product time starts from the first data time in the first data file plus hours. The data file must be defined before this command. A convenient way to synchronize the product with data. Inherited to member objects..

`HybridLevel = leveli`

Valid for: #Segment

As “Level” but using hybrid levels in data. See “Level”.

`HybridLevelPlace = leveli   xf   yf`

Valid for: #Segment

As “LevelPlace” but using hybrid levels in data. See “LevelPlace”.

`IncludeNightSymbols`

Valid for: SymbolParams

Does not work with current Weathersymbol3 mappings.

When mapping the correct symbol name for a specific weather condition, a symbol name with the postfix “_night” is used (added to the day symbol), if the sun has set.

I.e. instead of

```text
// cloud. form  th.%  p1h  temp fog sunElev
MultiParams =   79    57   260     353   4  327  511
MultiMapping = 0 65, -, 55 110, 2 99, -, -, -.83 99 heavyThunderShower
MultiMapping = 0 65, -, 55 110, 2 99                      heavyThunderShower_night
````

you may use:

```text
// cloud. form  th.%  p1h  temp fog
MultiParams =   79    57   260     353   4  327
MultiMapping = 0 65, -, 55 110, 2 99  heavyThunderShower
IncludeNightSymbols
```

`Indent =  widthf`

Valid for: GenericTexts

The first line in a paragraph will be indented “width” points.

`InitialScalingPoints = leftf  bottomf  rightf  topf`

Valid for: #Product, #Segment, #SubImage

When rescaling is used, this command gives the original scaling area. The coordinate points are given as in the original illustration. InitialScalingPoints is used in cooperation with CurrentScalingPoints. Together they define the rescaling from the initial to the current illustration. Scaling is independent in the two directions and offset can be replaced. Useful e.g. when a product get a new background map and the set of locations still would fit the new map as geographical places. The command pair can also be used when a fixed location set has been designed for standardized use on various backgrounds.

In #Subimage the InitialScalingPoints & CurrentScalingPoints pair is used to place the the image correctly. E.g. contur fields can be placed and scaled on maps according to map backgrounds.

`Language = Finnish | English |Swedish | Spanish | Germany | French | {Latvian|Lettish}  | Saame | Chinese`

Valid for: #Product, #Segment, #NameDay, GenericTimeTexts

Language to be used e.g. in time notifications (day of week, month) and also e.g. to choose right name day file. Saame and Chinese offer only limited functionality and requires special fonts. Inherited to member objects.

`Level = leveli`

Valid for: #Segment

Level value in hPa (850. 700, or whatever the indicator in the data has been set to). The individual levels for the “LevelTable” are listed by this command. In qdpress the levels replace the time steps, the processing matrix hence being of type locations levels parameters.

`LevelPlace = leveli   xf   yf`

Valid for: #Segment

(x,y) coordinates for the specified level. If the different levels are equally spaced the command “LevelTable” is more sensible.

`LevelTable = firstLocationLastXf   firstLocationLastYf   numberi`

Valid for: #Segment

A table presentation may be constructed having the level variable changing instead of the time (the time is fixed). The data must consequently recognize levels (850, 700, etc). The individual levels are given by the command “Level”.  The (x,y) steps between the levels are equally spaced from the object’s start point (first location) to the last point given as attribule values here.

`LineFeed =  heightfGenericTexts`

The linefeed is set to ”height” points. Enables exact fitting of a text into a specific column. Default is the nominal line feed of the font.

`LineFeedFactor =  factorfGenericTexts`

The linefeed is set to ”factor” * font_size. Because default line feed is clearly greater than font size, a value of 1 will narrow and e.g. 2 will broaden the line feed.

`LogText = logTexts`

Valid for: #Segment

A text to be put into the log file on the #Segment starting line. The log file is anyway separated into sections according to the #Segments. With this text you can document the function of the particular #Segment. By using a distinguishable string both the definition file and the log file are documented identically.

Example:

```text
Definition:
LogText = “Temperatures”

Log File output:
2. Segment (Temperatures)
```

`LongMinusshort`

Valid for: #Product, #Segment, #Number

The minus in negative numbers will be represented with the long dash. Is inherited to lower levels. Long dashes may not function for some fonts.

`LonLatCorners = firstLongf  firstLatf  secondLongf  secondLatf`

Valid for: #Map

See PointCorners. These are the corresponding real world coordinates for the two points. Longitudes and latitudes are in degrees and fractions of these (not minutes).

`LonLatCorner[s]Names = firstPlaces  secondPlaces`

Valid for: #Map

See PointCorners and LonLatCorners. Instead of latitude and longitude you may refer to geographical coordinates via names which have been defined in the Name2LongLat.txt file.

`LowerCase`

Valid for: GenericTexts, GenericParamTexts

All characters of the string will be lower case.

`MainHeaderFont = fontNames`

Valid for: #Text

Font for the main header part of a text. The HTML tag <h.> (any number) is used to mark the main header in the input text. This enables the use of one single text file including both headers and body text. See also HeaderFont.

`MainHeaderSize = sizef`

Valid for: #Text

Font size (in points) for the main header  part of a text. The HTML tag <h.> is used to mark the header in the input text. This enables the use of one single text file including both headers and body text.

`MainMap`

Valid for: #Segment

The map in this #Segment is declared to be the “MainMap”. It is referred to by the command “CoordinatesFromMainMap”.

`MakeElementsAfterSegments`

Valid for: #Product

The elements on Product level shall be drawn after the #Segments and all internal elements of the #Segments. By this option you may control overlapping of different elements of the entire product.

`MakeAfterSegments`

Valid for: GenericTexts, #SubImage, #FixedSymbol

The object shall be put after all #Segments and their contents in the output eps-file. Thus it is also presented on top of these in the illustration.

`MandatoryData = dataFileNames`

Valid for: #Product

Declares the “dataFileName” like the command “Data”, but also setting this particular data critical. Meaning that the program is interrupted if it is not found. See also “DataFilesCritical” and “DataFilesNotCritical”.

`Mapping = lowLimitf   highLimitf   {strings | None}`

Valid for: #Parameters members

When the parameter value is in the range “lowLimit” to “highLimit” the result of the operator is the string “string”. In #Symbol it is used as the file name to be copied into the illustration. In #Letter it is the string to be printed in the illustration. If the resulting “string” shall depend on more than one parameter “MultiMapping” is to be used.

`MappingValueToSubstitute = 3169n  (n=0-9)`

Valid for: #Substitute

The constant 3169n in a (multi)mapping schema is replaced by the value achieved by applying the parameter, location and date to the data. See #Substitute.

`Maximum = startHouri   endHouri`

Valid for: ParametersMember

Maximum value between startHour and endHour (utc). The hours are either absolute or relative (attribute RelativeTime declared) to the actual time of the object.  The hours may cross the date border. If AllowMissing is declared the value is calculated despite missing hours.

Example1:

```text
// Next night maximum
Maximum = 18 30
```

Example2:

```text
(Six hours time steps)
RelativeTime
Maximum = -2 +2 ->  maximum 4-8, 10-14, 16-20utc etc
````

`MaxLength = numberOfCharsi`

Valid for: #Text, #NameDay, #ExtremePlace

The string is truncated to numberOfChars if this length is exceeded. It functions slightly differently depending on object. In #Text the string is truncated exactly after numberOfChars, in #NameDay always at a space not leaving necessarily any characters at all, and in #ExtremePlace similarly but the first word is truncated exactly ending with a dot if needed.

`MaxNumber = numberOfNamesi`

Valid for: #NameDay

The string is truncated to numberOfNames words if needed. An alternative way to prevent the names from spreading too wide is the “MaxLength” command.

`Mean = startHouri   endHouri`

Valid for: #Parameters members

Mean value between startHour and endHour (utc). The hours are either absolute or relative (attribute RelativeTime declared) to the actual time of the object. The hours may cross the date border. If AllowMissing is declared the value is calculated despite missing hours.

Example1:

```text
// next night mean
Mean = 18 30
````

Example2:

```text
(Six hours time steps)
RelativeTime
Mean = -2 +2 ->  mean 4-8, 10-14, 16-20utc etc
````

`Minimum = startHouri   endHouri`

Valid for: #Parameters members

Minimum value from startHour to endHour (utc). The hours are either absolute or relative (attribute RelativeTime declared) to the actual time of the object. The hours may cross the date border. If AllowMissing is declared the value is calculated despite missing hours.

Example1:

```text
Minimum = 18 30 -> next night minimum
````

Example2:

```text
(Six hours time steps)
RelativeTime
Minimum = -2 +2 ->  minimum 4-8, 10-14, 16-20utc etc
````

`MissingValue = missingTexts`

Valid for: #Letter

A text to be substituted for missing values, e.g. `-` and `32700`.

`MultiMapping = lowLimitf [highLimitf] [,lowLimitf   [highLimitf]]…  string`

Valid for: SymbolParams, #Letter

Used for result values dependent on several parameters. The parameters shall be given in a “MultiParams” command prior to these “MultiMapping” lines. When all parameter values are in their  respective range (“lowLimit” to “highLimit”) the result is the string “string”. In #Symbol it is used as the file name to be copied into the illustration. In #Letter it is the string to be printed in the illustration. If the resulting “string” is depend on only one parameter the command “Mapping” can be used.  See also “Mapping” and “MultiParams”.

Example:

```text
MultiParams =    79     57    260        353     4   327  //FMI param codes
MultiMapping = 0 65,     - , 55 100 ,  2  99          heavy_thunder_shower
MultiMapping =  -     ,      - , 0  29    , 0 0.02, - ,2 fog
MultiMapping =  -     ,     6 ,                                    hail
````

Parameter conditions can be given as a single value or as a value range. Parameters are separated by commas. Unused parameters at the end of lines can be skipped, otherwise the are marked with “-“. The result is obtained from the first line fulfilling the conditions, despite further lines alike. If a parameter with given value(s) is missing, the whole result will be missing (unless a result has been reached on a previous line.

`MultiParams = paramNumberi …`

Valid for: SymbolParams, #Letter

The parameter identification numbers to be used in “MultiMapping”. See “Multimapping” including  example.

`Name =  files`

Valid for: #SubImage, #FixedSymbol, ParamSymbols

The image file name in the directory (symbol set) defined.

`NewFileMoving =  file  moveXf  moveYf`

Valid for: #SubImage

Initiates a new image and gives the coordinates to put it on. All attributes are inherited from the containing image but are transferred by (moveX, moveY) including clipping. Hence, when there are several similar e.g. temperature fields, only this line is needed for every new picture.

`NewImage`

Valid for: #SubImage

Initiates a new image. All attributes are inherited from the containing image. You must also give the file name and normally you also define new coordinates and clipping.

`NewName =  filef`

Valid for: #SubImage

Initiates a new image giving the file name. All attributes are inherited from the containing image. Normally you also define new coordinates and clipping.

`NewRelativelyWithTimestamp =  files  moveFromFirstXf  moveFromFirstYf  hourFromFirstYi`

Valid for: #SubImage

Initiates a new image read from “file”, the name starting with timestamp DDMM.  The location is moved by (moveFromFirstX, moveFromFirstY) from the present location. The time of the new object is achieved by adding hourFromFirstY to the mother objects time (shall match the timestamp).

`NoAlignment`

Valid for: #Text,  #Nameday

The text in a column IS NOT aligned on the right column border. By default it IS aligned. The left border is always aligned.  When aligned it is achieved by adjusting the word gaps.

`NotRot[ating]Range = startValyef  stopValuef`

Valid for: #RotatingSymbol, #WindDirection

A range within which the symbol will not rotate. The range refers to the primary parameter, not the rotating. E.g. winds with special symbol for nearly calm conditions, should not rotate the “calm” symbol. The range should be synchronized with the change from calm to normal symbol.

`NumberMapping = lowLimitf highLimitf valuef`

Valid for: #Number

Values between “lowLimit” and “highLimit” are presented as “value”. Only one range of this kind is allowed per #Number object.

`OnlyForMissingPrecedingImage`

Valid for: #SubImage, GenericTexts,

A special command which allows a text or another image to be used as substitute for a missing image. This object is related to the one coming in front of it in the definition code. Has been used e.g. to write “Image not available” when an image defined, not has been found.

`OnlySupplement`

Valid for: #Segment

The #Segment’s time steps are supplements for the former #Segment’s time steps (in the definition). Note that missing denotes to missing time steps, all their #Parameters’ members. With other data the symbols and numbers may succeed. The #Numbers in the #Segment to be supplemented can be declared with ”SupplementForMissing” to avoid missing marking.

`OptimizeGlobalObs`

Valid for: #Segment

(Do not use unless absolutely necessary)

Combines all commands involved in “presenting observations from the local midday all over the world”. Thus it will automatically include UseStationsLocalTime & UseBackupPreviousDay in #Segment, AllowMissing & Maximum = 10 18 in #Number, and UseBackupTime in #Station.

`Orientation = verticalLongitudef`

Valid for: #Map

The longitude being vertical on the map if suitable for the projection.

`OutputMode = EPS|Text`

Default: EPS

Valid for: #Product

Instead of eps-code it is possible to get the result as a standard text file. All applicable output are printed as ascii. For numbers and strings there are no problems, but e.g. images are just skipped. The application is not designed for this kind of output, so no guarantee is given for reasonable result, especially the order of parameters may not be very good.

`ParagraphBorders = leftf  topf  rightf`

Valid for: GenericTexts

The borders of a rectangular area inside which the text is written. Note that the lower border is left open.  The program cuts the text if it should exceed bottom of the plot area. Alternative to Place and AreaPath.

`ParagraphMove = yMovef`

Valid for: GenericTexts

The text will be moved by yMove. Tests indicates that only negative values function properly. Normally no need for this command, because you can adjust the original y coordinate for the text.

`Param[eter] =  fmiParameterNnumberi#ParametersMember`

The parameter number (FMI code) for data item to be presented.

`Parentheses | Parenthesis`

Valid for: #Number

The result value is included in parantheses..

`Path = pathf`

Valid for: #SubImage, #Text

The full path for the image or text.

`Place =  xf  yf`

Valid for: GenericTexts

The absolute (x,y) coordinates to place the string on. Note also alignment. Alternative to ParagraphBorders and AreaPath.

`PlaceMove =  xMovef  yMovef`

Valid for: ParamObjects, SegmentTexts, #Subimage

The relative (x,y) coordinates for the object. In proportion to the reference point of a #Segment loop. In SubImage it is the transfer from stored placement.

The command Place gives absolute coordinates and is primarily used on #Product level.

`PlaceOnMap =   [names]  xf  yf`

See: StationPlaceOnMap

`PointCorners = firstxf  firstyf  secondxf  secondyf`

Valid for: #Map

Two (x,y) points on the map (unit=points) which set mapping between real word (long,lat) and product coordinates (x,y). Both x and y distance should be reasonable big to ensure precision in calculations. It is convenient to choose e.g. two marked cities on the map to simplify future changes. The two (x,y) points represent  the same locations as in the command LonLatCorners or command LonLatNames, one of which is mandatory together with PointCorners.

`PressTimestamp = name[.suffix]s no timestamp`

Valid for: #Product

The name of the output file will be DDMMname (FMI standard). DDMM is the valid time for the illustration (first map). In Finland it is often tomorrow.

`PrimaryData[File] = [path]datafilenames`

Valid for: #Segment

A data file to be preferred to the one defined by “Data”. When the data value(s) are found from this one there are used, otherwise they are taken from the other. As all data files, this also must be declared on #Product level (Data =, not PrimaryData =).

`Probability`

(I don't know what this is. -ed.)

ParamSymbols

In-built option for one customer (Tiehallinto) have areal probabilities for some parameters.

`ProductName = name[.suffix]sdefinition_file.eps`

Valid for: #Product

The name of the output eps- file.

`ProductNameFormat = {string|Sta[tion] | sta[tion] | Lan[guage] | lan[guage] | Timestamp[Utc] |  DataTimestamp[Utc] | AnalysesTime}/…`

Valid for: #Product

A command that allows output file naming according to (first) station, language and times (Timestamp=wall clock, DataTimestamp=first used time or/and data analysis time) in arbitrary order. These can be combined with any text strings. Station and language can be written with three leading characters or full words, and with capital first letter or not. Two of the times can be in utc or as local time. One or two of the times can be defined precisely in the ProductNameTimeFormat and SecondProductNameTimeFormat.
Example:
ProductNameFormat = station/_/Lan/Timestamp/News_/DataTimeStamp
ProductNameTimeFormat = DDMM_
SecondProductNameTimeFormat = H
Output -> riga_Eng2601_News_14.eps

`ProductNameNumbering = startingNumberForManagers`

Valid for: #Product

A number starting from “ProductNameNumbering” is added to the output file for every file produced by the Manager loop. If there isn’t any other proper identifier (location, time) this option can be used. May also be defined in the Manager itself..

`ProductNameTimeFormat = timeFormat`

Valid for: #Product

The time format to be used in the output file name. If there are several time strings this is used for the first of them. See xxxx for timeFormat syntax.

`Projection = Polster | YKJ | Stereo | Equidistant`

Default: Polster

Valid for: #Map

The projection of the #Map.

`Randomization`

PLEASE DO NOT USE!

Valid for: ParamSymbols

The parameter values are randomized slightly around their true values. Instead of areas with constant values you can achieve some variations with this option. E.g. if temperatures are printed as integers and they are over a large area 17.5 degrees you can have one half of them as 18 and the other as 17 (on average). Randomization is performed linearly within the specified interval. Some of the parameters have preset randomization intervals:

* temperature: +-0.6 degrees
* cloudiness: +-12.5%-units staying within 0-100%
* precipitation amount: +-0.4 * value
* thunder probability: +-6%-units

For other parameters or when you want to change these you can define your own interval by “RandomizationInterval”.

Randomization obeys the “AllowRandom” setting in the season_status file. Thus randomization can be switched off from all products if needed.

`Random[ization]Interval = rangef`

Valid for: ParamSymbols

The range for parameter value randomization.

`RelativeHours = hoursFromNowi  resolutioni  [offseti]`

Valid for: #Product, #Segment

Starting time for the illustration (in continuous production) can properly be set by this command relatively to actual time. It is valid on the setting level and lower levels unless time attributes are changed. The time is achieved by adding hoursFromNow to the wall clock time (utc) and taking the next full hour (utc) divisible with resolution. Optionally the possible times can be shifted with offset hours.

Example1 (wall clock = 10utc in both):

```text
RelativeHours = 4 6  ->  18utc today
```

Example2:

```text
RelativeHours = 6 24 12  ->  12utc tomorrow
````

`RelativeSymbolSize =  scalef`

Valid for: #Segment

The images in all #Symbol objects are is scaled by this additional value.

`RelativeTime`

Valid for: #ParametersObjects

The hours given are relative to the #Segments’ time(s). Default is absolute.

Example:

```text
Hour = 12
TimeTable = 100 0 4 24
RelativeTime
Mean = -1 +1  ->
```

mean values of hours 11-13. Otherwise would be 23-01

`RGBColor =  rf  gf  bf`

Valid for: #Product, #Segment, #Parameters, GenericTexts, GenericParamTexts

RGB color for objects which obey color setting (texts etc). Color components are given in range 0 – 1. This command turns the color space to RGB, default is CMYK. RGB colors should be used for products to be shown on display screens. Inherited to lower levels.

`RGBMapping = lowLimitf   highLimitf   rf  gf  bf`

Valid for: GenericParamTexts

Values in the range “lowLimit” to “highLimit” are presented in the defined color.

`Rotate = 90`

Valid for: GenericTexts, GenericParamTexts

No other rotation angles than 90, are available yet.

Rotates the string 90 degrees (left to right -> down to up). The rotation is accomplished around the string’s alignment point (left, center or right) placed at the object’s (x,y) coordinates.

See also the following.

`Rotate =  anglef`

Valid for: #Symbol

Rotates the image with “angle” degrees around its origo. See also the previous.

`Rotation =  anglef   xCenterf   xCenterf`

Valid for: #Subimage

Rotates the image with “angle” degrees around its point (xCenter, yCenter). The center point can be approximate.

`RotationFirst =  anglef   xCenterf   xCenterf`

Valid for: #Subimage

As “Rotation” but rotation is performed before scaling and replacement.

`Rounding =  integerPrecisioni`

Valid for: ParamsSymbols

The value is rounded to the nearest integer divisible with “integerPrecision”.

Example:

```text
Rounding = 5  ->  3.7 = 5; 21.2 = 20.
````

`Scale =  xScalef   [yScalef]`

Valid for: #SubImage, #FixedSymbol, ParamSymbols, (#Product)

The image is scaled by “xScale” and “yScale” in respective directions. If only one value is given it is adapted in both directions. Equivalent to “SizeFactor”.

On #Product level the scaling effects most of the product, background, location lists, texts, but not e.g. #Subimages.

`ScalingParam[eter] = FmiParamNumberi`

Valid for: #ScaledSymbol

The parameter to use in #ScaledSymbol to perform the scaling. See also “HeightScale” and “WidthScale”.

`SecondProductNameTimeFormat = timeFormat`

Valid for: #Product

The second time format to be used in the output file name (see ProductNameFormat). This format is used for the secondly mentioned time in the command. See xxxx for timeFormat syntax.

`SegmentMove =  xMovef   yMovef`

Valid for: #Segment

The entire #Segment is transferred by “xMove” and “yMove” in respective directions. Can be used when parts of an illustration is moved to new (x,y) coordinates. The scale is not changed.

`ShortMinusshort`

Valid for: #Product, #Segment, #Number

The minus in negative numbers will be represented with the short dash. Is inherited to lower levels.

`Size =  heightf`

Default: 12

Valid for: GenericTexts, GenericParamTexts

Size of font in text objects (in point units). On higher levels the spelling “TextSize” should be used in order to avoid ambiguity.

`SizeFactor =  xScalef   [yScalef]`

Valid for: #SubImage, #FixedSymbol, ParamSymbols

See Scale.

`Station =  [wmoNumberi] names  [[longf  latf ] xf  yf] | None`

Valid for: #Segment

A location to be presented in the illustration. The name is mandatory. The parameters are the link between the data and the (x,y)-point in the illustration. The location  name here identifies the right piece of data in the file. Either the name is in the data (point data) or is found in the Name2LongLat file giving the long/lat values (GRID data). Optinally the long&lat values can be given in the command (may not work in all situations). If the Station command is in a table, the individual (x,y)-point should not be given (see StationTable).

The (x,y)-point is the reference place for the objects inside the #Parameters object, and can also be such for the objects on #Segment level. However, the latter can also be placed by absolute coordinates. Name is the string being printed out by #StationName.

None can be used in a table to left empy lines (to be filled in another #Segment)

`StationLonLat =  [wmoNumberi] names  longf  latf  xf  yf`

Valid for: #Segment

A location point at (x,y) showing data from (lon,lat). No #Map is needed but the data must be in GRID format. The name attribute is used only for printing of the name string.

`StationLonLatOnMap =   [names]  lonf  latf`

Valid for: #Segment

A location point placed by (lon,lat)-coordinates on the illustration. The data objects(s) will be placed on this point (+ object movements). A #Map must be defined to enable mapping from (lon,lat) to (x,y) and the data must be in GRID format.

`StationMove = xMovef  yMovef`

Valid for: #Segment

The defined (x,y) coordinates of the locations are moved with these values. Can be used e.g. if a chart in the illustration has been given a new place. If the scale also changes, the command pair InitialScalingPoints & CurrentScalingPoints shall be used. If a #Map has been used it shall not be changed, instead use the old #Map and the old station coordinates with a proper “StationMove”.

`StationNameOnMap =   names`

Valid for: #Segment

A location identified by name on the illustration. If the data is point data, the named point shall exist in the data. If GRID data, the name (together with real world coordinates) must be in the Name2LongLat file.

`StationNamesAfterParams`

Valid for: #Segment

The location name objects (strings) are put after the #Parameters objects into the output file, hence being presented on top of them. Sometimes needed in order not to have the name covered by symbols etc.

`StationNewNaming =  =  [wmoNumberi] dataNames  newNames  [[longf  latf ] xf  yf]`

Valid for: #Segment

As Station but renaming the location. This format is only relevant if the name is printed in the illustration (by #StationName). DataName is the name in the data and newName the string to put in the illustration. Can be used e.g. when the language is changed or there is space only for an abbreviation in the output.

`StationPlaceMatrixOnMap = leftf  bottomf  rightf  topf  stepXf  stepYf`

Valid for: #Segment

Setting a matrix of locations to be shown based on the (x,y) coordinate space in the illustration. Starting at (left, bottom) stepping (stepX, stepY) to (right, top) a grid is set up. A #Map must be defined in order to find right real world coordinates fron the data.

`StationPlaceOnMap =   [names]  xf  yf`

Valid for: #Segment

A location point placed by (x,y) coordinates on the illustration. The data objects(s) will be placed on this point (+ object movements). A #Map must be defined to enable mapping from (x,y) coordinates to data and the data must be in GRID format.

`StationTable =   xFirstf  yFirstf  [xLastf]  yLastf  numberi`

Valid for: #Segment

A table of locations, equally spaced in the illustration. The (x,y) coordinate pair is defined for the first and last location together with the number of locations. XLast can be omitted if it is equal to xFirst (vertical table). The number need not be the actual number of locations used, it is only used to calculate the step size between locations. Therefore you don’t have to adjust this command if new locations are added to the end, just add the new locations to the list (presuming there is space for them).

The locations to be shown are listed as Station commands in the order to be printed, and they shall come after this StationTable command. All Stations are tried to be plotted despite the number given in the command. If several columns is needed a separate StationTable command is given for each.

`StationTableActive =   startIndexi  stepi  stopIndexi`

Valid for: #Number

Only every “step” location, starting from “startIndex” and ending at “stopIndex” is processed in the location list. The command may refer to a table or to locations freely placed on a map. Note that the command can be used only in the object #Number.

Example:

```text
StationTableActive = 10 5 25

// locations 10, 15, 20, 25 are plotted
````

`Sum = startHouri   endHouri`

Valid for: ParametersObjects

The sum of the parameter values from startHour to endHour (utc). The hours are either absolute or relative (attribute RelativeTime declared) to the actual time of the object.  The hours may cross the date border.  If AllowMissing is declared the value is calculated despite missing hours.

Example1:

```text
Sum = 18 30 -> next night sum
````

Example2:

```text
(Six hours time steps)
RelativeTime
Sum = -2 +2 ->  sum 4-8, 10-14, 16-20utc etc
````

`SummerWinterChoise = summerFiles  winterFiles`

Valid for: #SubImage

The option enables image choice  depending on weather situation. It has been used for legends to alterbetween rain and snow symbols (all seldom has place in one legend). The dividing condition has been set to +2.5 degrees for the day +2 (an average for forecast maps). The region parameter is set here to “Center” (see “SummerWinterChoiseFocusing”). If no data is available summer/normal time status is used.

`SummerWinterChoiseFocusing = summerFiles  winterFiles[southwest | south | southeast | west | center| east | northwest | north | northeast |oulu | lappi]`

Valid for: #SubImage

As #SubImage but the weather dependency is focused on a particular part of the country. Possible countries are Finland and Latvia, which is specified by the language setting. “Oulu” and “Lappi” applies only to Finland and on the other hand “northwest”, “north” and “northeast” are missing from Finland.

`Sunrise`

Valid for: #Suntime

The #Suntime notation is specified to sunrise.

`Sunset`

Valid for: #Suntime

The #Suntime notation is specified to sunset.

`SupplementForMissing`

Valid for: #Parameters objects

A supplement for the parameter value is tried to found in the next #Segment should it be missing here. No missing mark either is used here.

`SymbolSet = symbolsets`

Valid for: #Product, #Segment,  GenericSymbols

The set (directory) from which to pick named symbols (primarily in object #Symbol). Can be overridden on lower levels. The symbols in the set must be stored in internal Illustrator format (.ia). The symbols must not contain gradient colors or character types (they must be vectorized).

`SymbolSet = moon_phases`

Valid for: #Symbol

A dedicated SymbolSet name which will initiate presentation of moon phase(s). It is fixed to 8 phases, and there is a dedicated  mapping (moon_phases8.inc) for them. The algorithm is embedded into the application.

Example:

```text
#Symbol
        SymbolSet = moon_phases
        #Include moon_phases8
````

`TempNotMean`

Valid for: ParametersMember

No mean value is calculated from the temperature. To find a weather symbol you may have defined the average of all parameters for hours +-2. With this option set, the temperature to use, is however

`Text = staticTexts`

Valid for: #Text

The text string itself to be written. When coded into the definition it will be static. For changing texts a link to a file should be used (command File). Strings including spaces must be enclosed by quotes (“).

`TextAlignment = Center | Left | RightCenter

Valid for: #Product, #Segment, #Parameters, GenericTexts, GenericParamTexts

Texts can be aligned to left, center or right. Vertically they are always aligned to center unless written into a column. Can be overridden on lower levels.

`TextMapping = lowLimitf   highLimitf   {strings | None}`

Valid for: ParametersMember

See “Mapping”.

`TextSize = textHeightf`

Default: 12

Valid for: #Product, #Segment, #Parameters, GenericTexts, GenericParamTexts

Size of text strings, unit in points. Is inherited to lower levels. Plain “Size” at an upper level would be ambiguous.

`TimePlace = deltaHoursi   absXf  absYf`

Valid for: #Segment

Another copy of the presentation objects (#PressParams and #Segment as applicable) at point (“absX”, “absY”) and “deltaHours” hours from initial (not previous) time. The coordinates stand for the first location. Note that the objects also are presented at the initial point with initial time. If the time steps are equally spaced the use of “TimeTable” is recommended because “TimePlace” requires a separate command for ever step. See also “TimePlaceMoving”.

Example with a matrix of 4 maps (same result as in “TimePlaceMoving” example):

```text
Station = Helsinki 100 300
TimePlace = 24 250 300
TimePlace = 48 100 200
TimePlace = 72 250 200
```

`TimePlaceMoving = deltaHoursi   deltaXf   deltaYf`

Valid for: #Segment

As “TimePlace” but the coordinates (deltaX, deltaY) are relative to initial (not previous) point.

Example (same result as in “TimePlace” example):

```text
Station = Helsinki 100 300
TimePlaceMoving 24 150 0
TimePlaceMoving 48 0 -100
TimePlaceMoving 72 150 -100
````

`TimeSeries = xStepf   yStepf   numberi   hourStepi`

Valid for: #Segment

As TimeTable but defining the steps directly instead of ending point.
Example (same result as in “TimeTable” example):
Station = Helsinki 50 100
TimeTable = 100 0 4 24

`TimeSeriesActive = startIndexi  stepi  stopIndexi`

Valid for: GenericTexts, GenericParamTexts

The object is active only for time steps according to these settings.

Example:

```text
TimeSeriesActive = 2 3 8 -> active steps = 2, 5, 8 of lets say 20 steps.
````

`Timestamp = name[.suffix]s no timestamp`

Valid for: #Product

The name of the output file will be as default nameYYYYMMDDHH00, or the according to the format given in the ProductNameTimeFormat command. The time is the local time.

`TimeTable = lastXf   lastYf   numberi   hourStepi`

Valid for: #Segment

Evenly spaced “number” presentations of the objects (#PressParams and #Segment as applicable) with the time step “hourStep”. The (x,y) steps are calculated as (last-initial)/(number-1). Coordinates refer to the first location. See also “TimeSeries”.

Example (same result as in “TimeSeries” example):

```text
Station = Helsinki 50 100
TimeTable = 350 100 4 24
````

`TrueLatitude = latitudef60`

Valid for: #Map

True latitude for applicable projections.

`UpperCase`

Valid for: GenericTexts, GenericParamTexts

All characters of the string will be upper case.

`UseDataStartTime`

Valid for: #Product, #Segment

The product presentation time starts from the first time in the first data file. The data file must be defined before this command. A convenient way to synchronize the product with data. Time attributes can be overridden on lower levels.

`UseGridAsStationPoints`

Valid for: #Segment

All GRID points in the data are used as presentation points. A #Map must be defined to enable mapping to corresponding (x,y) coordinates

`UseGridAsStationPointsCropping  = lefti  bottomi  righti  topi`

Valid for: #Segment

Only data points of a GRID coming inside the defined coordinate area, are shown. A #Map must be defined to enable mapping of data to corresponding (x,y) coordinates. Note that the parameters refer to the end product layout, rather than being e.g. a subgrid.

`UseBackupPreviousDay`

Valid for: #Segment, SegmentMembers

If a data is not available from the actual day, the same time from the day before is used. This is often the case when presenting observations around the world. When a product is produced in the afternoon, observation from the local midday have not yet arrived from the American continent. Then it is convenient to show the day before (and not e.g. the local morning observations). These cities will be marked with an asterisk if “StationName” is used. The asterisk should also be explained in the legend.

`UseStationsLocalTime`

Valid for: #Segment

The time applies to the respective geographic location’s local time, or nearest of them in the data. E.g. if the #Segment’s “Hour = 15” all observations are picked near the local max temperature time. A time period can also be defined for the day time.

`UtcOutput`

Valid for: TimeTexts

The time notation will be UTC and not the default local time.

`WeightedMean = startHouri   endHouri  startWeightf  centerWeightf  endWeightf`

Valid for: ParametersMember

As command `Mean` but the values along the time line can be weighted separately. Data times not falling on start/center/end times will get a weight linearily interpolated.

Example:

```text
WeightedMean = 6 18  1 3 2
(values = 5, 8, 15, 13, 11 (hour step =3h))  -> weights= 1 2 3 2.5 2 -> result=11.476
````

`ValueFactor = coefficientf`

Valid for: #Parameters members

The result is multiplied with “coefficient”.

`Version = versionNumberi`

Valid for: #Product

Some commands may perform differently earlier than at present. The newest version is 30 (1.1.2012). By setting to the newest version you will achieve up-to-date functioning while older product definitions (without Version=30) will give unchanged results. Normally you don’t have to set the version.

`VerticalLongitude = degreesf`

Valid for: #RotatingSymbol, #WindDirection

Used for wind symbols on (large scale) maps in e.g. polar stereographic projections. Giving the longitude, all wind symbols will point correctly in relation to local compass direction.

`WidthFactor =  factorf`

Valid for: GenericTexts, GenericParamTexts

The characters are made broader (“factor”>1) or more narrow (<1). Make direct use of the PS operator “scale”.

`WidthScale = zeroValuef  nominalValuef  maxValuef`

Valid for: #ScaledSymbol

The scale formulae to be applied to a #ScaledSymbol in x-direction. “NominalValue” denotes the stored size, zeroValue the value for which it goes to zero width, and maxValue the highest allowed data value. The scaled width is achieved according to: (max(value,maxValue)-nominalValue)/(nominal¬Value-zeroValue). See also “HeightScale” and “ScalingParam”.

`Windchill`

Valid for: ParametersMember

Temperature and wind speed presented as a wind chill factor. According to the FMI formulae.

`YearData[File] = dataFiles`

Valid for: #Product

As command “Data”, but the data is used without the year variable (although it is there), only the date is valid. Can be used in relation to e.g. climatic data.

`YearPeriod = firstYeari  lastYeari`

Valid for: ParametersMember

The period to be calculated for data spanning over several years (fixed day).

`ZeroWithMinus`

Valid for: #Number

Values exactly zero are marked as minus-values.

## Ordinary Commands in the Manager

In addition to these commands the directives, comments and #Includes (but not $#variable) are allowed in the Manager. The commands in the Manager always overwrite corresponding attribute values defined in the definition file (pre-file) as suitable. But because usually only one attribute is given here, it affects the first object in a list, if the original definition comprises several. The commands are not in alphabetical order, but rather related to usability.

`MakeProduct`

The most essential command in the Manager. It will process the product in the actual stage (as defined by the pre-file and updated via the manager). Without this command no illustration is produced if the Manager is in use. Between various MakeProduct commands at least one other command shall be given in order not to have identical products (in fact this seems to be prevented giving an error message). If all attributes have proper values in the pre-file, you can start the Manager with “MakeProduct”.

`#End`

An #End must close the Manager interpretation. Otherwise the process will end up in a loop (although all products will be produced and the loop will be stopped shortly). Note the “#”.

`Station = [stationWmoi] names [lonf latf]`

The (first) station in all #Segments and other objects as applicable is changed to this location. Identical illustrations for many locations may typically be produced like this:

```text
Station = Helsinki  MakeProduct
Station = Turku     MakeProduct
Station = Riga        MakeProduct
#END
````

`StationNewNaming = [stationWmoi] names newNames [lonf latf]`

Like “Station” but the location is given another name than in the data. Only usable if the name is printed in the illustration or used in the output file name.

`StationAndFilenames = [stationWmoi] names [lonf latf]`

As Station above, but the new station name also affects the output file name, assuming station is included in the file name.

`AddHours = hoursi`

The time in all objects is changed by “hours” hours. Identical illustrations for several days may typically be produced like this:

```text
MakeProduct
AddHours = 24  MakeProduct
AddHours = 24  MakeProduct
AddHours = 24  MakeProduct
#END
````

`RelativeHours = hoursFromNowi  resolutioni`

Like the corresponding pre-file command without offset. The time is achieved by adding hoursFromNow to the wall clock time (utc) and taking the next full hour (utc) divisible with resolution.

`AddHours = hoursi`

The time in all objects is changed by “hours” hours. Identical illustrations for several days may typically be produced like this:

`Language = Finnish | English |Swedish | Spanish | Germany | French | {Latvian|Lettish}  | Saame | Chinese`

The language for all objects is changed to this.
Language setting is used e.g. in time notifications (day of week, month) and also e.g. to choose right name day file. Saame and Chinese offer only limited functionality and requires special fonts. Inherited to member objects.

Identical illustrations in several languages can be produced like this:

```text
MakeProduct
Language = English  MakeProduct
Language = Latvian  MakeProduct
#END
````

`Data = dataFiles`

The data file in the #Product is changed to “dataFile”, and hence the default for #Segments. The “dataFile” must however be declared in the pre-file like any other data to be used.

`Product[Name] = name[.suffix]s`

The output file name. Has the same function as in the pre-file but manager-commands always overwrite pre-file commands.

`ProductNameFormat = {string|Sta[tion] | sta[tion] | Lan[guage] | lan[guage] | Timestamp[Utc] |  DataTimestamp[Utc] | AnalysesTime}/…`

See commands in qdpress.

`CloseLogFile`

Printing into the log is stopped. When making several  similar products, with only location or time changing, it is often useless to duplicate almost identical information into the log file.

`ProductNameNumbering = startingNumberi`

The output files are numbered starting from name+“startingNumber” and incremented by one.

Example:

```text
ProductName = Meteogram_
ProductNameNumbering = 1
AddHours = 24  MakeProduct
AddHours = 24  MakeProduct
#END
```

output files:
Meteogram_1.eps
Meteogram_2.eps

`StopNumbering`

The product numbering is stopped, which was started by “ProductNameNumbering”.

`DeActivateFirstElement`

The first object  (not #Segment) defined in the pre-file is deactivated (completely bypassed). This command enables convenient differentiating between successive products on object level, when you work with the manager. Activating/deactivating has nothing to do with comments (they permanently block code). See also the next commands.

`ActivateFirstElement`

The first object (not #Segment) defined in the pre-file is reactivated. Reactivated because initially all objects are active and can be deactivated. See also the next commands.

`DeActivateFirstActiveElement`

The first ACTIVE object (not #Segment) defined in the pre-file is deactivated. Enables omitting of more than one object in the definition. See also the other activating/deactivating commands.

`ActivateFirstPassiveElement`

The first PASSIVE object (not #Segment)  defined in the pre-file is reactivated. Reactivated because initially all objects are active. See also the other activating/deactivating commands

`DeActivateFirstSegment`

The first #Segment defined in the pre-file is deactivated (completely bypassed). This command enables convenient differentiating between successive products on #Segment level, when you work with the manager. Activating/deactivating has nothing to do with comments (they permanently block code). See also the next command.

`ActivateFirstSegment`

The first #Segment defined in the pre-file is reactivated. Reactivated because initially all #Segments are active and can be deactivated. See also “DeActivateFirstSegment”.

`[Area]Mask = masks`

Used e.g. for the customer Tiehallinto. Seems to be slightly different meaning between “AreaMask” and “Mask”.

## Parting words

And that's how the cookie crumbles! If you read all of this, it's time for a coffee break.
