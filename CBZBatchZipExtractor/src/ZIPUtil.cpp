#include "ZIPUtil.h"
#include "zip_file.hpp"

static void zipAllinFolder(const std::string& folderPath, const std::string& zipPath)
{
    miniz_cpp::zip_file file;

    for (auto const& dir_entry : std::filesystem::directory_iterator{ folderPath })
    {
        std::string fileName = relative(dir_entry, folderPath).string();
        file.write(folderPath + "/" + fileName, fileName);
    }
    
    file.save(zipPath);
    std::filesystem::remove_all(folderPath);

}

static void unzipAllinFolder(const std::string& folderPath, const std::string& zipPath)
{
    miniz_cpp::zip_file file(zipPath);

    std::filesystem::create_directory(folderPath);

    file.extractall(folderPath);

    std::filesystem::remove(zipPath);
}

bool convertExtenstions(const std::string& baseDir, const char* oldExt, const char* newExt)
{
    if (baseDir != "" && std::filesystem::is_directory(baseDir)) {
        
        for (auto const& dir_entry : std::filesystem::directory_iterator{ baseDir })
        {
            std::string fileName = relative(dir_entry, baseDir).string();
            if (fileName.find(oldExt) != std::string::npos)
            {
                rename((baseDir + "/" + fileName).c_str(), 
                    (baseDir + "/" + fileName.substr(0, fileName.find(oldExt)) + newExt).c_str());
                PRINTLOG("Renamed " << fileName);
            }
        }
    }
    else
        PRINTLOG("That is not a Directory!");
    return 1;

}

bool zipAllinDirectory(const std::string& baseDir, const std::string& fileExt)
{
    if (std::filesystem::is_directory(baseDir)) {

        bool hasTitleCheck = 0;
        bool mangadesk = 0;

        if (baseDir.find("mangadesk") != std::string::npos)
        {
            char rep;
            mangadesk = 1;
            PRINTLOG("Does the folder have title included (y/n):");
            std::cin >> rep;

            if (rep == 'y' || rep == 'Y') hasTitleCheck = 1;
            else hasTitleCheck = 0;
        }

        for (auto const& dir_entry : std::filesystem::directory_iterator{ baseDir })
        {
            std::string oldFolderName = relative(dir_entry, baseDir).string();
            std::string newFolderName = oldFolderName;
            if (mangadesk && std::filesystem::is_directory(baseDir + "/" + oldFolderName))
            {
                newFolderName = oldFolderName.substr(0, oldFolderName.length() - 11);
                if (hasTitleCheck)
                {
                    newFolderName.replace(newFolderName.find(" [en-data] "), 11, " - ");
                }
                else
                {
                    newFolderName.replace(newFolderName.find(" [en-data] "), 11, "");
                }
            }
            
            std::string folderPath = baseDir + "/" + oldFolderName;
            std::string zipPath = baseDir + "/" + newFolderName + "." + fileExt;
            if (std::filesystem::is_directory(folderPath))
            {
                if (std::filesystem::exists(zipPath))
                    newFolderName += "(copy)";
                PRINTLOG("Creating Folder: " << newFolderName);
                PRINTLOG("Zipping");
                PRINTLOG("Folder Path: " << folderPath);
                PRINTLOG("Zip Path: " << zipPath);

                zipAllinFolder(folderPath, zipPath);

                PRINTLOG("Folder Zipped!!");
            }
            //folderList.emplace_back();
        }
        PRINTLOG("Finished Zipping!!\n");
    }
    else
        PRINTLOG("That is not a Directory!\n");
    return 1;
}

bool unzipAllinDirectory(const std::string& baseDir)
{
    if (std::filesystem::is_directory(baseDir)) {

        for (auto const& dir_entry : std::filesystem::directory_iterator{ baseDir })
        {
            std::string zipName = relative(dir_entry, baseDir).string();
            std::string folderName = zipName.substr(0, zipName.find_last_of('.'));
            if (std::filesystem::is_regular_file(FULLPATH(baseDir, zipName)))
            {
                PRINTLOG("Extracting");
                PRINTLOG("Zip Path: " << FULLPATH(baseDir, zipName));
                PRINTLOG("Folder Path: " << FULLPATH(baseDir, folderName));

                unzipAllinFolder(FULLPATH(baseDir, folderName), FULLPATH(baseDir, zipName));

                PRINTLOG("Folder Extracted!!");
            }
            //folderList.emplace_back();
        }
        PRINTLOG("Finished Extracting!!\n");
    }
    else
        PRINTLOG("That is not a Directory!\n");
    return 1;
}

std::string getDirectory()
{
    std::string getDir;
    PRINTLOG("Write Directory or Drag and Drop the folder:");
    std::getline(std::cin, getDir);
    if (getDir.find('"') != std::string::npos)
        return getDir.substr(1, getDir.length() - 2);
    else 
        return getDir;
}

char getOption()
{
    char getOp;
    std::cin >> getOp;
    std::cin.ignore();
    return getOp;
}

bool chooseFunction(const char getOp, const std::string& getDir)
{
    switch (getOp)
    {
    case '1':
        return zipAllinDirectory(getDir, "cbz");
        break;
    case '2':
        zipAllinDirectory(getDir, "cbr");
        break;
    case '3':
        convertExtenstions(getDir, "cbz", "cbr");
        break;
    case '4':
        convertExtenstions(getDir, "cbr", "cbz");
        break;
    case '5':
        unzipAllinDirectory(getDir);
        break;
    default:
        PRINTLOG("::Invalid input::\nExiting Application");
        break;
    }
}