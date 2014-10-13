import os
import string

prefixs=["ui_", "moc_", "qrc_"]
suffixs=[".aps", ".opt", ".ncb", ".pch", "sbr", ".bsc", ".obj", ".suo", ".sdf", ".user", ".tlog", ".lastbuildstate", ".pdb", ".idb", ".ilk", ".log", ".ipch", ".unsuccessfulbuild"]

for path,dirs,files in os.walk(".\\"):
    for file in files:
        bDelete = False
        for prefix in prefixs:
            if file.startswith(prefix):
                bDelete = True
        for suffix in suffixs:
            if file.endswith(suffix):
                bDelete = True
        if bDelete == True:
            fullPath = path + "\\" + file;
            if path == ".\\":
                fullPath = path + dir
            print fullPath
            os.remove(fullPath)