#pragma once
#include <filesystem>
#include <string>
#include <commdlg.h>
#include <span>

namespace File {
struct Data {
public:
    [[nodiscard]] std::string OpenFileDialog() const
    {
        // common dialog box structure, setting all fields to 0 is important
        char szFileName[MAX_PATH]{};

        OPENFILENAMEA ofn{
            .lStructSize = sizeof(ofn),
            .hwndOwner   = nullptr,
            .lpstrFilter = "Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0\0",
            .lpstrFile   = szFileName,
            .nMaxFile    = MAX_PATH - 1,
            .Flags       = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST,
        };
        return GetOpenFileNameA(&ofn) ? ofn.lpstrFile : std::string{};
    }
    void ScanFiles(std::string_view directory = "musics", std::string_view file_extension = ".wav") noexcept
    {
        file_list.clear();
        std::error_code ec;
        for (int64_t i64FileCount = 0; const auto& entry : std::filesystem::recursive_directory_iterator(
                                               directory,
                                               std::filesystem::directory_options::skip_permission_denied,
                                               ec)) {
            if (ec) {
                std::cerr << std::format("Filesystem Error: {}\n", ec.message());
                files_scanned = false;
                return;
            }

            if (entry.is_regular_file() && entry.path().extension() == file_extension) {
                file_list.push_back(entry.path().string());
                std::cout << std::format("Found {} File: {}\n", i64FileCount, file_list[i64FileCount]);
                i64FileCount++;
            }
        }
        files_scanned = true;
    }
    std::span<const std::string> GetFileList() const noexcept
    {
        return file_list;
    }
    bool IsScanned() const noexcept
    {
        return files_scanned;
    }

private:
    std::vector<std::string> file_list;
    bool                     files_scanned = false;
};
} // namespace File