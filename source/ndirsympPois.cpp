//----------------------------------------------------------------------------
FmiBoolean	NFmiDirectionSymbolParamRect::WritePS( const NFmiRect & AbsoluteRectOfSymbolGroup
										,NFmiQueryData* theQueryData
										,ofstream theDestinationFile)	
{

   itsDefToProductScale.SetEndScales(AbsoluteRectOfSymbolGroup.ToAbs(NFmiRect(TopLeft(),BottomRight())));

	if (!theQueryData->Param(theDataIdent()))
		return kFalse;

	float value = theQueryData->FloatValue();
	NFmiString symbolFile = *itsMapping->Map(value);

	if(symbolFile != NFmiString("None"))
	{
		NFmiString fileName = theSymbolDir;
	   fileName += NFmiString(*itsSubDir) += NFmiString("/")
	            += symbolFile += NFmiString(".eps");
		ifstream inFile = ifstream(fileName, ios::in);
		FmiBoolean tempBool = NFmiWritePSConcatDirection(itsDefToProductScale, value, theDestinationFile);
		tempBool = NFmiCopyFile(inFile,theDestinationFile);
		tempBool = NFmiWritePSEnd(theDestinationFile);
//		inFile.close();
	}
	return kTrue;
};
