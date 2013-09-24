import os, sys , jpype
DataType = ['BOOLEAN','BYTE','SHORT', 'INT','LONG','UBYTE','USHORT','UINT',    'ULONG','FLOAT',    'DOUBLE',    'ASCII_STRING''UTF_STRING','OTHER','DISCARD','MIXED']

ext_classpath = "D:\Working space\MyEclipse\swe-common-core-0.8.2.jar" 
jvmArg = "-Djava.class.path = " + ext_classpath  
jpype.startJVM(jpype.getDefaultJVMPath(), jvmArg)
SWEData = jpype.JClass('org.vast.sweCommon.SWEData')

data = raw_input()
datasource = SWEData(data)

print datasource.getDataComponent()
datasource.setDataComponent(data)
