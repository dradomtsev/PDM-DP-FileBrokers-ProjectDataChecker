#pragma once

struct Datastructure
{
	std::vector<std::basic_string<TCHAR>> FoldersToRead;
	std::vector<std::basic_string<TCHAR>> FoldersIgnore;
	std::vector<std::basic_string<TCHAR>> RootFolders;
};

struct Timing
{
	std::vector<std::basic_string<TCHAR>> ChkStartDateTime;
	std::vector<std::basic_string<TCHAR>> ChkEndDateTime;
};

struct Projects
{
	std::vector<std::basic_string<TCHAR>> Include;
	std::vector<std::basic_string<TCHAR>> Exclude;
};

struct Common
{
	std::basic_string<TCHAR> FileMask;
	std::basic_string<TCHAR> Cyrillic;
	std::basic_string<TCHAR> Company;
	std::basic_string<TCHAR> Role;
	std::basic_string<TCHAR> Stage;
};

struct Specific
{
	std::basic_string<TCHAR> FindDStageShaPubZZ;
	std::basic_string<TCHAR> FileMaskDStageShaPubZZ;
	std::basic_string<TCHAR> MaskDCompanyShaPubZZ;
};

struct Rules
{
	Common Common;
	Specific Specific;
};

struct FNS2
{
	Projects Projects;
	Rules Rules;
};

struct FNS3
{
	Projects Projects;
	Rules Rules;
};

struct Filenamingsystem
{
	 FNS2 FNS2;
	 FNS3 FNS3;
};

struct Connection
{
	std::basic_string<TCHAR> HostnamePort;
	std::basic_string<TCHAR> Login;
	std::basic_string<TCHAR> Password;
	std::basic_string<TCHAR> DefaultSchema;
};

struct Errors
{
	std::basic_string<TCHAR> Name;
	std::vector<std::basic_string<TCHAR>> Fields;
};

struct Fileextensions
{
	std::basic_string<TCHAR> Name;
	std::vector<std::basic_string<TCHAR>> Fields;
};

struct Files
{
	std::basic_string<TCHAR> Name;
	std::vector<std::basic_string<TCHAR>> Fields;
};

struct Folders
{
	std::basic_string<TCHAR> Name;
	std::vector<std::basic_string<TCHAR>> Fields;
};

struct Users
{
	std::basic_string<TCHAR> Name;
	std::vector<std::basic_string<TCHAR>> Fields;
};

struct Companies
{
	std::basic_string<TCHAR> Name;
	std::vector<std::basic_string<TCHAR>> Fields;
};

struct Roles
{
	std::basic_string<TCHAR> Name;
	std::vector<std::basic_string<TCHAR>> Fields;
};

struct Subsystems
{
	std::basic_string<TCHAR> Name;
	std::vector<std::basic_string<TCHAR>> Fields;
};

class Datatypes
{

	std::basic_string<TCHAR> Name;
	std::vector<std::basic_string<TCHAR>> Fields;
};

struct Tables
{
	Errors Errors;
	Fileextensions FileExtensions;
	Files Files;
	Folders Folders;
	Users Users;
	Projects Projects;
	Companies Companies;
	Roles Roles;
	Subsystems Subsystems;
	Datatypes DataTypes;
};

struct Database
{
	 Connection Connection;
	 Tables Tables;
};






struct INI_JSON
{
	Datastructure DataStructure;
	Timing Timing;
	Filenamingsystem FileNamingSystem;
	Database DataBase;
};;



