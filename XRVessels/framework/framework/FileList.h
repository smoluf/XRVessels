/**
  XR Vessel add-ons for OpenOrbiter Space Flight Simulator
  Copyright (C) 2006-2025 Douglas Beachy

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <https://www.gnu.org/licenses/>.

  Email: mailto:doug.beachy@outlook.com
  Web: https://www.alteaaerospace.com
**/

// ==============================================================
// Parses a tree of files, optionally recursing subdirectories,
// and invoking an abstract callback method for each file and folder.
// ==============================================================

#pragma once

#include <atlstr.h>		// for CString
#include <vector>

using namespace std;

class FileList
{
public:
    FileList(const char *pRootPath, const bool bRecurseSubfolders);
    FileList(const char *pRootPath, const bool bRecurseSubfolders, const char *pFileTypeToAccept);
    FileList(const char *pRootPath, const bool bRecurseSubfolders, const vector<CString> &fileTypesToAccept);
    virtual ~FileList();

    static bool DirectoryExists(const char *pPath)
    {
        DWORD dwAttrib = GetFileAttributes(pPath);

        return (dwAttrib != INVALID_FILE_ATTRIBUTES && (dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
    }

    // Scan (or rescan) file tree.
    // Returns true on succeess, or false if the root path does not exist or is not a directory.
    bool Scan()
    {
        if (!DirectoryExists(m_rootPath))
            return false;

        Scan(m_rootPath, 0);
        return true;
    }

    // Invoked for each file or folder node found; should return true if file node should be included or folder should be
    // recursed into, or false if the node should be skipped.
    virtual bool clbkFilterNode(const char *pPathOfNode, const WIN32_FIND_DATA &fd);

    // Callback invoked for non-empty file nodes that passed the clbkFilterNode check; this is here for subclasses to hook.
    virtual void clbkProcessFile(const char *pFilespec, const WIN32_FIND_DATA &fd);

    int GetScannedFileCount() const { return static_cast<int>(m_allFiles.size()); }
    bool IsEmpty() const { return m_allFiles.empty(); }
    const vector<CString> &GetScannedFilesList() const { return m_allFiles;  }
    const CString &GetRootPath() const { return m_rootPath; }

    // returns a random file entry from the list that is not a repeat of the previous one (provided there are at least two files in the list).
    const CString GetRandomFile();

    // returns a file entry from the list at the specified index (0..GetScannedFileCount()-1)
    const CString GetFile(const int index) const;

    // Returns the first file in the list with the specified basename.
    const CString *FindFileWithBasename(const char *pBasename) const;

protected:
    void Scan(const char *pPath, const int recursionLevel);

    CString m_rootPath;
    bool m_bRecurseSubfolders;
    vector<CString> m_fileTypesToAccept;
    int m_previousRandomFileIndex;  // 0..GetScannedFileCount()-1

    vector<CString> m_allFiles;     // full path of all files in the tree, starting with pRootPath.  This is m
};