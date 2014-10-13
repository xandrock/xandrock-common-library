import os
import string
import shutil

for path,dirs,files in os.walk(".\\"):
    for dir in dirs:
        if dir == ".svn":
            fullPath = path + "\\" + dir
            if path == ".\\":
                fullPath = path + dir
            print fullPath
            shutil.rmtree(fullPath)