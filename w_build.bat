set VCTargetsPath=c:\Program Files (x86)\MSBuild\Microsoft.Cpp\v4.0\V120

msbuild ..\MetaEngine.sln /v:n /p:Configuration=Lib
msbuild ..\MetaEngine.sln /v:n /p:Configuration=tests
