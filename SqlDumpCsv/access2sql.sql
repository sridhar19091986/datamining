USE [master]
GO

/****** Object:  Database [RedianHedian]    Script Date: 04/07/2011 16:46:10 ******/
IF  EXISTS (SELECT name FROM sys.databases WHERE name = N'RedianHedian')
DROP DATABASE [RedianHedian]
GO

USE [master]
GO

/****** Object:  Database [RedianHedian]    Script Date: 04/07/2011 16:46:10 ******/
CREATE DATABASE [RedianHedian] ON  PRIMARY 
( NAME = N'RedianHedian', FILENAME = N'E:\dbfile\RedianHedian.mdf' , SIZE = 51200KB , MAXSIZE = UNLIMITED , FILEGROWTH = 5120KB )
 LOG ON 
( NAME = N'RedianHedian_log', FILENAME = N'E:\dbfile\RedianHedian.ldf' , SIZE = 5120KB , MAXSIZE = 2048GB , FILEGROWTH = 5120KB )
GO


USE [RedianHedian]
GO
SELECT * 
INTO gprsredian
FROM 
OPENDATASOURCE ('Microsoft.Jet.OLEDB.4.0','Data Source="g:\FG.mdb";User ID=Admin;Password=' )...fg