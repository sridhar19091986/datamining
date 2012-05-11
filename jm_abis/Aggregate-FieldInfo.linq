<Query Kind="Statements">
  <Connection>
    <ID>a116a106-fc11-40b0-a229-fcdf8e0f71a9</ID>
    <Persist>true</Persist>
    <Driver Assembly="IQDriver" PublicKeyToken="5b59726538a49684">IQDriver.IQDriver</Driver>
    <Provider>Devart.Data.MySql</Provider>
    <CustomCxString>AQAAANCMnd8BFdERjHoAwE/Cl+sBAAAA0/N/oBPNRE2LBeTenlt7UgAAAAACAAAAAAADZgAAqAAAABAAAAC1yMySD8P+X26O0gHTVxlUAAAAAASAAACgAAAAEAAAAPC7eTlT9aEMwICX4OAeuyhYAAAAlOIbULzXimR4H10mS/lteCoqiiYseRrIOf+P0afwt8mK0eqveSolEc7mOR/BzyYXzfGNrDCYPMnfPm40FJVJ7pv+g9PfVrzfK6jTHYvb8GXjzr44V9QCUhQAAAB07ye88abgvNwB7BcLmiojNbVzuw==</CustomCxString>
    <Server>192.168.4.209</Server>
    <Database>mpos_main_testing</Database>
    <UserName>hantele</UserName>
    <Password>AQAAANCMnd8BFdERjHoAwE/Cl+sBAAAA0/N/oBPNRE2LBeTenlt7UgAAAAACAAAAAAADZgAAqAAAABAAAAC+y19UApoWqxeIo3ba3h5eAAAAAASAAACgAAAAEAAAAFYnRbJ7MUsHICixuTO9W24IAAAAsUP4lmEbPzwUAAAA/N787LxHbRkbGrz0msbME7NTbPM=</Password>
    <EncryptCustomCxString>true</EncryptCustomCxString>
    <DisplayName>Server=192.168.4.209;Database=mpos_main_testing;uid=hantele;pwd=hantele;Persist Security Info=True;charset=utf8;</DisplayName>
    <DriverData>
      <StripUnderscores>false</StripUnderscores>
      <QuietenAllCaps>false</QuietenAllCaps>
      <Port>3306</Port>
    </DriverData>
  </Connection>
  <Reference>G:\htmlconvertsql\SqlCompact.v.2011.05.21\Soccer Score Forecast\Soccer Score Forecast\bin\Release\HtmlAgilityPack.dll</Reference>
  <Reference>D:\Program Files\Microsoft SQL Server\100\SDK\Assemblies\Microsoft.SqlServer.Types.dll</Reference>
  <Namespace>HtmlAgilityPack</Namespace>
  <Namespace>Microsoft.SqlServer.Types</Namespace>
  <IncludePredicateBuilder>true</IncludePredicateBuilder>
</Query>

FieldInfo[] atts = typeof (MCOMCARRIER).GetFields();

atts.Select(e=>e.Name).Aggregate ("MCOMCARRIER:", (seed, n) => seed +","+ n).Dump();