Syntax for writing .pre and .prem files.

Mostly copy-pasted from original Word-document and propably contains a lot of errors.
Feel free to fix any mistakes that you notice.

## Syntax in this document

syntax        | meaning
--------------|----------------------------------------------------
`[option]`    | Optional syntax is in brackets
`[option]...` | One or more optional items depicted with three dots

***

```
#!PressProduct

/* Multiline comment starting and
 ending  /* Can also be nested */  
*/

// Line comments

[$#globalVariable]...

[ #Weekday OnlyIn SnowPeriod ]

[ #ifWinterTime -or- #ifNotWinterTime
  definitions
[ #else
  definitions]
#endif ]…

[ #ifSnowPeriod -or- #ifNotSnowPeriod
  definitions
[ #snowPeriodElse
  definitions]
#snowPeriodEndif ]…

[ #ifPollenPeriod -or- #ifNotPollenPeriod
  definitions 
[ #pollenPeriodElse
  definitions]
#pollenPeriodEndif ]…

[ #ifPollenOrSnowPeriod
  pollen or snow period definitions
[ #pollenOrSnowPeriodElse
otherwise definition]
#pollenOrSnowPeriodEndif ]…

[ #ifSummer -or- #ifNotSummer  (default 31.5-31.8)
  definitions 
[ #summerElse
  definitions]
#summerEndif ]…

[ #if<weekday>
  definitions for this weekday
[ #<weekday>Else
  definitions for other weekday]
#<weekday>Endif ]…
(<weekday> = Monday -or- Tuesday -or- Wednesday -or- Thursday
      -or- Friday -or- Saturday -or- Sunday) 

[ #ifConditionDay[<Ww>]…      (n days e.g. #ifConditionDayMoFrTh)
  definitions for these weekdays
[ #[<Ww>]…Else                                                              (#MoFrThElse)
  definitions for the other weekdays]
#[<Ww>]…Endif ]…                                                        (#MoFrThEndif)
(<Ww > = Mo -or- Tu -or- We -or- Th -or- Fr -or- Sa -or- Su)
(must be introduced in .pre file if inside any #Include) 
 
[ #ifConditionHour<hour>
”hour reached”-definitions
[ #conditionHour< hour >Else
” hour not reached”-definitions]
#conditionHour < hour >Endif ]…
(<hour> = 0………23 (SA))
(must be introduced in .pre file if inside any #Include) 

[ #ifConditionDate<DD1MM1>-<DD2MM2>
”inside period”- definitions
[ #conditionDate<DD1MM1>-<DD2MM2>Else
”outside period”- definitions
# conditionDate<DD1MM1>-<DD2MM2>Endif ]…
(must be introduced in .pre file if inside any #Include) 

[ #Include [path]Filename[.suffix]]…      (also nested)

*** Manager-shared-part ends ***

Version = version number (newest=20, effecting only [initial/currentscalingPoints + 	#Map] and Texts)

[OutputMode = EPS -or- Text] 

[Product[Name] = output-filename[.suffix]]
-or-
[Timestamp = output-filename[.suffix] ]
-or-
[PressTimestamp = output-filename[.suffix] ]
-or-
[DataTimestamp = output-filename[.suffix]]                                   
-or-
[ProductNameFormat = part-name -or- Sta[tion] -or- sta[tion] -or- 
             Lan[guage] -or- lan[guage]-or- Timestamp[Utc]  -or- 
             DataTimestamp[Utc]  -or- AnalysesTime  [/theSames]…] 
             (different  results for short/long formats)       

[ProductNameNumbering = startingNumberForManager] 	          

[ProductNameTimeformat = timeFormat] 	            *1*			
[SecondProductNameTimeformat = timeFormat] 	*1*			
[MakeElementsAfterSegments]                                                          
[DeleteScandinavianFromProductName]
[[All]DataFilesCritical]  (onward)
[DataFilesNotCritical]  (onward)
[Data[file] = datafile] …
[YearData[File] = datafile]        (applies for all years; e.g. climatology)
[MandatoryData[File] = datafile] …
[Background -or- MapFile = epsFile] …
[OutDir[ectory] = directory]
[LongMinus]

[SymbolSet = symbolDirectory]
[AdditionalSymbolScale = add.scale] (if all are rescaled)
[Font = font]                    (can be overriden)
[TextSize = textHeight]   (can be overriden)
[TextAlignment = [Center -or- Centre -or- Middle] -or- Left -or- Right 
                                                                                       (can be overriden)
[AvoidOrphanSyllables]

[Scale -or- SizeFactor = coefficient] (do not affect #SubImages) 
-or-
[InitialScalingPoints =  leftBottomX leftBottomY rigthtTopX  rightTopY
CurrentScalingPoints = leftBottomX leftBottomY rigthtTopX rightTopY]

[BoundingBox -or- BoundingBorder =  leftBottomX leftBottomY 
                                                                 rigthtTopX  rightTopY]
[ClippingRectangle = leftBottomX leftBottomY rigthtTopX  rightTopY]
[GifRectangle =     leftBottomX leftBottomY rigthtTopX  rightTopY]
[Language = Finnish -or- Swedish -or- English -or- Chinese -or- Spanish -or- Germany -or- French -or- Saame -or- [Latvian -or- Lettish]]      

{ [Hour = utc]
[Day = dayOffset] }
-or-
[Date = day month year]
-or-
[UseDataStartTime]
-or-
[HoursFromData = hours ] 				    
-or-                                                                                                           
[RelativeHours = hoursFromNow resolution [offset]                         

[[CMYK]Color = c  m  y  k]        (0.-1.)
-or-
[RGBColor = r  g  b]                   (0.-1.)

AreaMask = mask]    
					
 [#Map 
	[Projection = YKJ -or- Polster -or- Stereo -or- Equidi[stant]]               

	[Orientation = verticalLongitude]
PointCorners = leftBottomX leftBottomY rigthtTopX rightTopY

LonLatCorner[s]Names = bottomLeftPlace  topRightPlace
	-or-	
LonLatCorners = leftBottomLon leftBottomLat rigthtTopLon rightTopLat
	-or-	
WorldCorners = leftBottomX leftBottomY rigthtTopX rightTopY
[TrueLatitude] = latitude]
#MapEnd]  (the only one to be ended)

[#SubImage[s]
	File -or- FileWithTimestamp = [path/]file
	-or-	
SummerWinterChoise  = summerFile  winterFile
	-or-
SummerWinterChoiseFocusing  = summerFile  winterFile [southwest  -or- south -or- southeast -or- west -or- [middle -or- centre -or- center] -or- east -or- northwest -or- north -or- northeast]
 
[OnlyForMissingPrecedingImage ]

	[Directory = directory ]
	-or-	
[Path = path]

[Day = dayOffset ]
[Hour = utc ]

[Rotation = angle xCenter yCenter]   (center not precise)
	-or-	
[RotationFirst = angle xCenter yCenter] 

[PlaceMove  =  x  y ]
	-or-	
[Scale -or- SizeFactor = xyCoefficient]
	-or-	
[InitialScalingPoints =  leftBottomX leftBottomY rigthtTopX rightTopY
  	 CurrentScalingPoints =  leftBottomX leftBottomY rigthtTopX rightTopY]
			
[Clip[pingRectangle] =   left  bottom  right  top ]
	-or-	
[CurrentScalingAndClipping =   left  bottom  right  top]
	-or-	
	[ClippingPath = [x y]… ] 	 (min 3 points)		

[NewImage]…
[NewName  = file]…
[NewFileMoving]  = file xMove yMove]
-or-
[NewRelativelyWithTimestamp  = 
                    file xMoveFromFirst yMoveFromFirsta timeFromFirst]…

[MakeAfterSegments -or- MakeAtEnd]                                                         
] …
[#Text                                                                                (P=column, M=other)
	{File = [path/]file					                       PM
	[Timestamp] 							           PM
            -or-                               
 	[Directory = directory ] 						PM                               
	 -or-
	[Path = path]}							            PM
	 -or-
	Text = theText						                       PM

[OnlyForMissingPrecedingImage ]      				PM

	[MaxLength = numberOfChar]				            PM	

	Place = x y						                       M 
-or-
ParagraphBorders = left  top  right                                                P
-or-					
AreaPath -or- AreaBorders = {x y}…  (min 3 points)                    P

	[Size =  height]						             	PM
	[WidthFactor = coefficient]	  (e.g. 1.06)				PM
[Rotate = 90 ]					              	            M
	[Font = font]					                                    PM
	[Alignment = [[Center -or- Centre -or- Middle]-or-Left -or- Right] M
[[CMYK]Color = c  m  y  k] (0.-1.)                                                    PM
-or-
[RGBColor = r  g  b]        (0.-1.)                                                     PM
	[ParagraphMove=  yMove]				            	P
	[Indent = width]							P
	
	[HeaderFont = font ]			                                                P	
	[HeaderSize =  height ]						P
	[MainHeaderFont = font ]		                                                P	
	[MainHeaderSize =  height]					                P

[LineFeed = height]							P
	-or-
[LineFeedFactor = coefficient]					P
[NoLineFeed]						

	[UpperCase -or-				                                   PM
	LowerCase  -or- 			                                               PM   
	Capitalize]							           PM			
	[AddInFront = text]							PM
	[AddAfter = text]						            PM
	[CharSpaceJustification = coeff.]    (e.g. -0.5, 0.2)			PM
	[AvoidOrphanSyllables]						P
	[NoAlignment]                                                                                   P

	[#SubTimeText                                                                		M    
		#TimeText commands applicable]…
	[#SubText                                                             		            M    
		# Text commands applicable]…
	[#SubComputerTime                                                             	M    
		# ComputerTime commands applicable]…	

	[MakeAfterSegments -or- MakeAtEnd]                                        PM                                                        
] …
[#Time[Text] 
	[UtcOutput] 	                                                                    
	Place = x y

	[OnlyForMissingPrecedingImage]
	[Size = height]
	[WidthFactor = coefficient]	  (e.g. 1.06)	
	[Day = dayOffset]					   
	[Hour = utc] 							   
	[Rotate = 90 ]
	[Format = timeFormat] 		*1*
	[Font = font]
	[Alignment = [[Center -or- Centre -or- Middle]-or-Left -or- Right]
[[CMYK]Color = c  m  y  k]                                                               
-or-
[RGBColor = r  g  b]                                                                           
	[Indent = width]
							
[LineFeed = height]							
	-or-
	[LineFeedFactor = coefficient]	
					
	[ParagraphMove = yMove]
	[CharSpaceJustification = coeff.]    (e.g. -0.5, 0.2)

	[UpperCase -or-
	LowerCase  -or-
	Capitalize]							

	[AddInFront = text]
	[AddAfter = text]

	[#SubTimeText                                                                	  
		#TimeText commands applicable]…
	[#SubText                                                             		            
		# Text commands applicable]…
	[#SubComputerTime                                                               
		# ComputerTime commands applicable]…

[MakeAfterSegments -or- MakeAtEnd]                                                                                             
] …
[#ComputerTime
	[UtcOutput] 	                                                                    
	Place = x  y
	
	[OnlyForMissingPrecedingImage]
	[Size = height]
	[WidthFactor = coefficient]	  (e.g. 1.06)	
	[Day = dayOffset]					   
	[Hour = utc] 							   

	[Rotate = 90 ]
	[Format = timeFormat] 		*1*
	[Font = font]
	[Alignment = [[Center -or- Centre -or- Middle]-or-Left -or- Right]
[[CMYK]Color = c  m  y  k]                                                               
-or-
[RGBColor = r  g  b]                                                                           
	[Indent = width]
							
[LineFeed = height]							
	-or-
	[LineFeedFactor = coefficient]	
	
	[ParagraphMove =  yMove]
	[CharSpaceJustification = coeff.]    (e.g. -0.5, 0.2)
											
	[UpperCase -or-
	LowerCase  -or-
	Capitalize]							

	[AddInFront = text]
	[AddAfter = text]

	[#SubTimeText                                                                	  
		#TimeText commands applicable]…
	[#SubText                                                             		            
		# Text commands applicable]…
	[#SubComputerTime                                                               
		# ComputerTime commands applicable]…

[MakeAfterSegments -or- MakeAtEnd]                                                                                             
] …
[#NameDay                                                                                   
[Language = Finnish -or- Swedish -or- Latvian]
[Day = dayOffset] 
	[MaxNumber = numberOfNames ]
	[MaxLength = numberOfChars ]

	Place = x y						                       
-or-
ParagraphBorders = left  top  right                                               

[OnlyForMissingPrecedingImage ]      		
	[Size =  height]						            
	[WidthFactor = coefficient]	  (e.g. 1.06)				
[Rotate = 90]
[Font = font]					                                    
	[Alignment = [[Center -or- Centre -or- Middle]-or-Left -or- Right] 	[[CMYK]Color = c  m  y  k] (0.-1.)                                                    
-or-
[RGBColor = r  g  b]        (0.-1.)                                                     
	[ParagraphMove = [xMove]  yMove]				            
	[Indent = width]								
	[LineFeed = height]							
-or-
[LineFeedFactor = coeff.]	
					
	[ParagraphMove =  yMove]	
	[[CharSpaceJustification = coeff.]    (e.g. -0.5, 0.2)

	[UpperCase -or-
	LowerCase  -or-
	Capitalize]
							
	[NoAlignment]

	[AddInFront = text]
	[AddAfter = text]

	[#SubTimeText                                                                	  
		#TimeText commands applicable]…
	[#SubText                                                             		            
		# Text commands applicable]…
	[#SubComputerTime                                                               
		# ComputerTime commands applicable]…

	[MakeAfterSegments -or- MakeAtEnd]
 
[#FixedSymbol 
	Place = x  y

	[Scale -or- SizeFactor = xCoeff.  [yCoeff.]

	SymbolSet = directory
	Name = imageFile
	[MakeAfterSegments -or- MakeAtEnd]
] …
[#FixedSymbolLocations 
	{Place = xArvo  yArvo}…
	#FixedSymbol 
		[MakeAfterSegments -or- MakeAtEnd]
		[Scale -or- SizeFactor = xCoeff.  [yCoeff.]

		SymbolSet = directory
		Name = imageFile
] …

[#Segment 
	[LogText = textIntoLogFile]  	         
	[LogTextAdd = TextAdditionToStationnameInLogFile] 
	[SegmentMove = xMove  yMove] 

[OptimizeGlobalObs]  (->UseStationsLocalTime;
	UseBackupPreviousDay; in #Number AllowMissing;
	Maximum = 10 18; in #Symbol UseBackupTime)                                                                                                                             
 
[AreaOperation]                                                                                                                              

	[ONLYSUPPLEMENT] 

	[Data[file] = file]  
	[PrimaryData[file] = file]  
	[DataNotNeeded]                                                                                    
[UseStationsLocalTime]
[UseBackupPreviousDay]                                        
                                        
	[Language = Finnish -or- Swedish -or- English -or- Chinese -or- Spanish 	-or- Germany -or- French -or- Saame -or- [Latvian -or- Lettish]]      

[Font = font]                       (can be overridden on lower levels)
[TextSize = height]            (can be overridden on lower levels)
[TextAlignment = [[Center -or- Centre -or- Middle]-or-Left -or- Right] [LongMinus]

 [[CMYK]Color = c  m  y  k]                                                               
-or-
[RGBColor = r  g  b]                                                                           

[{[Hour = utcHour]
  [Day = dayOffset] }
-or-
DayAddition = daysAddedToHighestLevel                                                       
-or-
UseDataStartTime                                                      
-or-
HoursFromData = hours 
-or-
 	RelativeHours = hoursFromNow resolution [offset]
	]

[MainMap]      (the one in this #Segment)                               

	[#Map 
		[Projection = YKJ -or- Polster -or- Stereo -or- Equidi[stant]]               

		[Orientation = verticalLongitude]
	PointCorners = leftBottomX leftBottomY rigthtTopX rightTopY

	LonLatCorner[s]Names = bottomLeftPlace  topRightPlace
		-or-	
	LonLatCorners = leftBottomLon leftBottomLat 
		rigthtTopLon rightTopLat
		-or-	
	WorldCorners = leftBotX leftBotY rigthtTopX rightTopY
	[TrueLat[itude] = latitude]
	#MapEnd]  

[RelativeSymbolSize = coeff.] 

	[[StationTable = xFirst yFirst [xLast] yLast number]
	[BackupStation]  (the next here)
	{Station = [wmoNumber] name [[lon lat]  x  y ] -or- None
     -or- 
StationNewNaming = [wmoNumber] dataName  newName [[lon lat] x y]  
     -or- 
	StationLonLat = [wmoNumber] name lon lat x y     
     -or-                                                              
StationLonLatNewNaming = [wmoNumber] dataName  newName [lon lat  
							[ x  y] ]
      -or-                                                              
[Station]PlaceOnMap = [name]  x  y 
      -or-                                                              
StationNameOnMap = name -or- None                                                         
      -or-                                                              
StationLonLatOnMap = [name] lon  lat              
} …
] …

 [UseGridAsStationPoints]    
-or-                                                                   
[UseGridAsStationPointsCropping] = x1 y1 x2 y2  (on output layout)                                                                  

[StationPlaceMatrixOnMap] = xstart ystart xend yend xdelta ydelta

[AreaOperation]                                                                                                                                          

[StationMove -or- StationRelocation =   xMove  yMove 
  -or-
 {InitialScalingPoints =   bottomLeftX  bottomLeftY  topRightX  topRightY
  CurrentScalingPoints = bottomLeftX  bottomLeftY  topRightX  topRightY }
  -or-
  CoordinatesFromMainMap
]
[ {TimeTable = firstStationLastX firstStationLastY  number timeStep } 
  -or/and-
{TimePlace = hoursFromFirst  firstStationX  firstStationY (absolute)
-or-
TimePlaceMoving = hoursFromFirst  xFromFirst  yFromFirst 
                                             (relative)        (after TimePlace?)
-or-
TimeSeries = xStep  yStep  number  hourStep}  
]                              

[ {LevelTable = xLast  yLast  number }              
  -or/and-
   {LevelPlace = level  x  y}… 	 
]
[Level = level]…               (850 500 …)       				 

[{HybridLevel = hybridLevel}…
-or/and- 					 
{HybridLevelPlace = hybridLevel  x  y}…
]
[DistanceCheck = xmin ymin]  (only symbols and elements after them)
[StationNamesAfterParams]      
					 
[#Time[Text] as above without Day and Hour, but additionally
[AnalysingTime]	                                                     	                                                                                         	[TimeseriesActive = startIndex step [stopIndex]]
[PlaceMove = xMove yMove]
] …

[#Text  as above ] …
[#FixedSymbol  as above ] …      
[#Nameday as above  +
[PlaceMove = xMove yMove]  ] …
[#DayChangeText  as #Text above ] …  (not implemented yet, now “*”)

[#StationName
	[PlaceMove = xMove yMove ] 

		[Size = height] 
		[WidthFactor = coefficient]	  (e.g. 1.06)				
	[Rotate = 90]					                        			[Font = font]					                                    
		[Alignment = [[Center -or- Centre -or- Middle]-or-Left -or- Right] 		[[CMYK]Color = c  m  y  k] (0.-1.)                                                    
	-or-
	[RGBVColor = r  g  b]        (0.-1.)                                                     
		[[CharSpaceJustification = coeff.]    (e.g. -0.5, 0.2)

		[UpperCase -or-
		LowerCase  -or-
		Capitalize]
							

		[AddInFront  = text]
		[AddAfter  =  text]
		[AddLocalTime]
	] …
[#StationNumber]…  as #StationName  (wmoNumber)

[#Parameters
[[CMYK]Color = c  m  y  k]                                                               
-or-
[RGBColor = r  g  b]                                                                           

		[Font = font]
		[Alignment = [[Center -or- Centre -or- Middle]-or-Left -or- Right]
[TextSize = height]     

	[#Symbol 
[Param[eter] = FmiParameterNumber]				
	[Hour = utcHour]
		[RelativeTime]
	                                                                                        	       				[TimeSeriesActive = startIndex step [stopIndex]]
		[BypassValue = value] 	                                          				       
	[Probability]          		                  
	[AllowMissing]          		                   

	[AreaMean]          		                   
		 -or-        
	[AreaSum]          		                            
		 -or- 
	[AreaMinimum]          		                               
		 -or- 
	[AreaMaximum]          		                               

	[Maximum = startHour  endHour]         		
		 -or- 
 	[Minimum = startHour  endHour] 			
		 -or- 
  	[Sum = startHour  endHour] 		       
		 -or- 
	[Mean = startHour  endHour] 		
		 -or- 
[WeightedMean = startHour  endHour startWeight
                                            centerWeight endWeight] 
	
		[AcceptanceInterval = min  max] 
		[YearPeriod = firstYear  lastYear] 
            [ValueFactor = coeff.]

		[PlaceMove = xMove yMove ]  
		[PlaceMoveAlternating =  xMove yMove xAltAdd  yAltAdd ] 
		[Scale -or- SizeFactor = xCoeff.  [yCoeff.]] 
		
		[SymbolSet = directory] (MoonPhases->option MoonPhases)
		
[TempNotMean] 

		[
		Mapping = rAlkuLuku [rLoppuLuku] tiedosto		
 -or-
{
		MultiParam[eter]s = [parameter ]…                         
{MultiMapping = lowLimit [highLimit] [,lowLimit                      			[highLimit]]…      file}…
			]

			[IncludeNightSymbols]  (use ”…_night” named) 
			[Rotate = angle]
		[UseBackupPreviousDay]
[DegreeDays]                                         
		[WindChill]
[Randomization                                                                                                   
   -or-
  		Random[ization]interval = valueRange]  
		[Rounding = integerprecision]
		[SupplementForMissing]      (in next #Segment)
[EquiDistanceMarking = valueInterval  +-acceptanceRange]  (in multMappig first;   conturs)		                         	
[EquiDistanceAndAdjusting = valueInterval  affectingRadiusInPoints] (conturs)   
 	] …

	[#RotatingSymbol
		RotatingParam[eterl = FmiParameterNumber
		[NotRot[ating]Range = startValue stopValue] 
		[VerticalLongitude = degrees]
		+ all #Symbol commands
] …
	[#WindDirection]…
		as #RotatingSymbol but not 
Parameter or RotatingParameter                                                  
] …

	[#ScaledSymbol                                                  
HeightScale = zeroSize storedSize maxSize
WidthScale = zeroSize storedSize maxSize
ScalingParam[eter] = FmiParamNumber
                         + all #Symbol commands			       
	] …

	[#Number
[Param[eter] = FmiParameterName]				
	[Hour = utcHour]
		[RelativeTime]
		[Moonphase]
	                                                                                        	       				[TimeSeriesActive = startIndex step [stopIndex]]
		[BypassValue = value] 	                                          				       
	[Probability]          		                  
	[AllowMissing]          		                   

	[AreaMean]          		                   
		 -or-        
	[AreaSum]          		                            
		 -or- 
	[AreaMinimum]          		                               
		 -or- 
	[AreaMaximum]          		                               

	[Maximum = startHour  endHour]         		
		 -or- 
 	[Minimum = startHour  endHour] 			
		 -or- 
  	[Sum = startHour  endHour] 		       
		 -or- 
	[Mean = startHour  endHour] 		
		 -or- 
[WeightedMean = startHour  endHour startWeight
                                            centerWeight endWeight] 
	
		[AcceptanceInterval = min  max] 
		[YearPeriod = firstYear  lastYear] 
            [ValueFactor = coeff.]

		[Hour = utcHour]
		[Format = numberFormat] 			*2*
		[Font = font]

		[PlaceMove = xMove yMove ]  
		[Size = height] 

			[[Text]Mapping = lowLimit highLimit {text -or- None}] …                
			[NumberMapping = lowLimit highLimit value] (only one!)                
 			[CMYKMapping = lowLimit highLimit c  m  y  k ] …    
			[RGBMapping = lowLimit highLimit r  g  b ] …    

			[Alignment = [[Center -or- Centre -or- Middle]-or-Left -or- 					Right]

[[CMYK]Color = c  m  y  k]                                                               
-or-
[RGBColor = r  g  b]                                                                           
		[UseBackupPreviousDay]
[TempNotMean] 
[Parentheses]
			[ZeroWithMinus]
			[DotToComma]

			[AddInFront = text]						
			[AddAfter = text]
						            
[LongMinus
 -or-
[ShortMinus]
		
[DegreeDays]                                         
		[FromMeanWindToMax]                                               
		[Windchill]                                               
 		[Fahrenheit]                                                             
  		[DetachSign]     
		
[Randomization                                                                                                   
   -or-
  		Random[ization]interval = valueRange]  
                                                                                                   
          		[UseFromStorageInFrontOf]                                                                                                   
                                        
                                    (these 4 can be useful in text output)
		[PutInStorage]                                                                                            
		[UseFromStorage]                                                                                                   
		[UseFromStorageConditionally]                                                                                                                    
		[StorageQueue = queueNumber]                                                                                                                                           
		
		[Rounding = integerPrecision]
		[SupplementForMissing]      (in next #Segment)
[EquiDistanceMarking = valueInterval  +-acceptanceRange]  (in multMappig first par;   conturs)		                         	
[EquiDistanceAndAdjusting = valueInterval  affectingRadiusInPoints] (conturs)   
[AddStationName]
		[StationTableActive = startIndex step [stopIndex]]
		[TimeSeriesActive = startIndex step [stopIndex]]
[WidthFactor = coeff. ]  (e.g. 1.2)                                                                          
] …

	[#WindSpeed]…
		as  #Number but not Parameter
	
[#Substitute  (help object for climatology, before #Text where used)
		Param[eter] = parameter 
MappingValueToSubstitute = 3169n (n=0-9)
	]…

	[#Text ]    (see main level)

	[#Letter
[Param[eter] = FmiParameterName]				
	[Hour = utcHour]
		[RelativeTime]
		[Moonphase]
	                                                                                        	       				[TimeSeriesActive = startIndex step [stopIndex]]
		[BypassValue = value] 	                                          				       
	[Probability]          		                  
	[AllowMissing]          		                   

	[AreaMean]          		                   
		 -or-        
	[AreaSum]          		                            
		 -or- 
	[AreaMinimum]          		                               
		 -or- 
	[AreaMaximum]          		                               

	[Maximum = startHour  endHour]         		
		 -or- 
 	[Minimum = startHour  endHour] 			
		 -or- 
  	[Sum = startHour  endHour] 		       
		 -or- 
	[Mean = startHour  endHour] 		
		 -or- 
[WeightedMean = startHour  endHour startWeight
                                            centerWeight endWeight] 
	
		[AcceptanceInterval = min  max] 
		[YearPeriod = firstYear  lastYear] 
            [ValueFactor = coeff.]

		[Hour = utcHour]
		[Format = numberFormat] 			*2*
		[Font = font]

		[PlaceMove = xMove yMove ]  
		[Size = height] 

			[Alignment = [[Center -or- Centre -or- Middle]-or-Left -or- 					Right]

[[CMYK]Color = c  m  y  k]                                                               
-or-
[RGBColor = r  g  b]                                                                           
			
			[CMYKMapping = lowLimit highLimit c  m  y  k ] …    
			[RGBMapping = lowLimit highLimit r  g  b ] …    

			{Mapping = lowLimit highLimit text } …  
-or-
{
		MultiParam[eter]s = [parameter ]…                         
[MultiMapping = lowLimit [highLimit] 
    [, lowLimit [highLimit] ]…  text]…
			} 
		[MissingValue = missingText]     (e.g. -)                                            

			[UpperCase -or-
			LowerCase  -or-
			Capitalize]
			[AddInFront = text]
			[AddAfter = text]

[LongMinus -or-
ShortMinus]

		[UseBackupPreviousDay]
[TempNotMean] 
[DegreeDays]                                         
		[WindChill]                                               
[Randomization                                                                                                   
   -or-
  		Random[ization]interval = valueRange]  

		
		[Rounding = integerPrecision]
		[SupplementForMissing]      (in next #Segment)
[EquiDistanceMarking = valueInterval  +-acceptanceRange]      	(in multMappig first;   conturs)       	
[EquiDistanceAndAdjusting = valueInterval  	affectingRadiusInPoints] (conturs)   
			[Extremes = maxMark minMark] (e.g. pressure H L)
[AddStationName]
[WidthFactor = coeff. ]  (e.g. 1.2)                                                                          
] …
	 [#FixedSymbol
		[PlaceMove = xMove yMove]  
		[Scale -or- SizeFactor = [width]  height ] 
		Rotate = angle
		SymbolSet = directory
		Name = imageFile
 ] …
 [#Time[Text]  
[FinnishTimeZone]                                                                           
                        [PlaceMove = xMove  yMove]  
			[Language = Finnish -or- Swedish -or- English -or- 
			Chinese -or- Spanish -or- Germany -or- French -or- 
			 Saame -or- [Latvian -or- Lettish]]      

[Size = height]  
		[Hour = utc]  
		[Format = timeFormat]    	*1*  
			[Alignment = [[Center -or- Centre -or- Middle]-or-Left -or-
				 Right]
[[CMYK]Color = c  m  y  k]                                                               
 -or-
[RGBColor = r  g  b]                                                                           
		[AddInFront = text]  
		[AddAfter = text]  
		[Font = font]  
		[TimeSeriesActive = startIndex  step  [stopIndex]] 
[WidthFactor = coeff. ]  (e.g. 1.2)                                                                          
 
] …
[#SunTime]                                                                 
			[Sunrise -or- Sunset]
			+ #TimeText commands
] …
[#ExtremeTime] ?
			#TimeText commands
] …
[#ExtremePlace]                                                                  
 [MaxLength = numberOfChars]  
		[FillWithUnderscore] 
	+ #Text as applicable
] …
	]
] …
#END

*1*  timeFormat:

The strings below are dedicated for the following time notes (assume the time to be 7.3.1999 at 9:43):
		YYYY ->	1999
		YY		99
		MM		03
		M		3
		DD		07
		D		7
		HH		09
		H		9
		mm		43   obs small (capital -> month)
		I		5 (fr) (chinese weekdays by font)                                       
		Wwww, wwww, WWWW	Friday, Friday, FRIDAY
		Www, www, WWW		Fri, fri, FRI
		Ww, ww, WW		Fr, fr, FR
		Nnnn, nnnn, NNNN		March, march, MARCH
		Nnn, nnn, NNN		Mar, mar, MAR		
		Tttt, tttt, TTTT [+-n]   	Tomorrow, tomorrow, TOMORROW *)
		anything else                	as it
		\anything\			as it, even these time codes

		Examples:
		"done D.M.YY at HH:mm" -> ”done 7.3.99 at 09:23”
		"D.M.\YY\ H:mm" -> “7.3.YY 9:23”   (\not coded\)
		“On wwww HH o’clock” -> ”On friday 09 o’clock” 
		“tttt wwww” ->  today Friday 
		*) compared to this day; if a newspaper is disributed tomorrow, +1
                         shall be used, and  so on for consecutive days

You may use #SubTimeText etc to concatenate more than one time into a text.

*2*  mumberFormat 
format		         data = 12.3 
%.0f			12
%.2f			12.30
%+.1f			+12.3
%(+).0f		12     (only below 10 have +)
%.(1)f			12     (only below 1 with decimal)
negative values of course always signed
 
Syntax for the Manager (.prem file)                                           1.11.2011/LW
							               
*** See also the shared directives in the beginning ***

[OutputMode = EPS -or- Text]                               
{{ Product[Name] = output-filename[.suffix
-or-
ProductNameFormat = part-name -or- Sta[tion] -or- sta[tion] -or- 
             Lan[guage] -or- lan[guage]-or- Timestamp[Utc]  -or- 
             DataTimestamp[Utc]  -or- AnalysesTime  [/theSames]… 
-or-
ProductNameNumbering = startingNumber 	          
-or-
StopNumbering				          
-or-
Station = [stationWmo] name [lon lat]
-or-
StationAndFileNames = [stationWmo] name [lon lat]                                  
-or-                                                                                                                StationNewNaming = [wmoNumber] dataName  newName [lon lat]   
-or-
Language = Finnish -or- Swedish -or- English -or- Chinese -or- Spanish -or- Germany -or- French -or- Saame -or- [Latvian -or- Lettish]  
-or-
RelativeHours = hoursFromNow resolution                         
-or-
AddHours = hours    				
-or-
Data = file  				
-or-
AreaMask = maskArea                                                                        	
-or-
ActivateFirstElement	
-or-
DeActivateFirstElement	
-or-
ActivateFirstPassiveElement	
-or-
DeActivateFirstActiveElement	
-or-
ActivateFirstSegment	
-or-
DeActivateFirstSegment	
-or-
CloseLogfile }…

MakeProduct }…
#END  				
```
	
