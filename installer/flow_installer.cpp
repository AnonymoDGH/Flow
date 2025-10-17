#include <windows.h>
#include <commctrl.h>
#include <shlobj.h>
#include <winhttp.h>
#include <string>
#include <fstream>
#include <filesystem>
#include <vector>

#pragma comment(lib, "comctl32.lib")
#pragma comment(lib, "winhttp.lib")
#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

namespace fs = std::filesystem;

// Global variables
HINSTANCE hInst;
HWND hMainWnd;
HWND hProgressBar;
HWND hStatusLabel;
HWND hInstallBtn;
HWND hCancelBtn;
HWND hPathEdit;
HWND hBrowseBtn;
std::wstring installPath = L"C:\\Program Files\\Flow";

// GitHub configuration
const wchar_t* GITHUB_USER = L"AnonymoDGH";
const wchar_t* GITHUB_REPO = L"Flow";
const wchar_t* GITHUB_BRANCH = L"main";

// Function declarations
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void InstallFlow();
bool IsAdmin();
bool AddToPath(const std::wstring& path);
void CreateUninstaller(const std::wstring& installPath);
void UpdateStatus(const std::wstring& status);
void SetProgress(int value);
bool DownloadFile(const std::wstring& url, const std::wstring& outputPath);

// Check if running as administrator
bool IsAdmin() {
    BOOL isAdmin = FALSE;
    PSID adminGroup = NULL;
    SID_IDENTIFIER_AUTHORITY ntAuthority = SECURITY_NT_AUTHORITY;
    
    if (AllocateAndInitializeSid(&ntAuthority, 2, SECURITY_BUILTIN_DOMAIN_RID,
        DOMAIN_ALIAS_RID_ADMINS, 0, 0, 0, 0, 0, 0, &adminGroup)) {
        CheckTokenMembership(NULL, adminGroup, &isAdmin);
        FreeSid(adminGroup);
    }
    
    return isAdmin == TRUE;
}

// Add to PATH environment variable
bool AddToPath(const std::wstring& path) {
    HKEY hKey;
    if (RegOpenKeyExW(HKEY_LOCAL_MACHINE, L"SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Environment",
        0, KEY_READ | KEY_WRITE, &hKey) != ERROR_SUCCESS) {
        return false;
    }
    
    wchar_t buffer[32768];
    DWORD bufferSize = sizeof(buffer);
    DWORD type;
    
    if (RegQueryValueExW(hKey, L"Path", NULL, &type, (LPBYTE)buffer, &bufferSize) != ERROR_SUCCESS) {
        RegCloseKey(hKey);
        return false;
    }
    
    std::wstring currentPath(buffer);
    
    // Check if already in PATH
    if (currentPath.find(path) != std::wstring::npos) {
        RegCloseKey(hKey);
        return true;
    }
    
    // Add to PATH
    std::wstring newPath = currentPath + L";" + path;
    
    if (RegSetValueExW(hKey, L"Path", 0, REG_EXPAND_SZ, (LPBYTE)newPath.c_str(),
        (newPath.length() + 1) * sizeof(wchar_t)) != ERROR_SUCCESS) {
        RegCloseKey(hKey);
        return false;
    }
    
    RegCloseKey(hKey);
    
    // Broadcast change
    SendMessageTimeout(HWND_BROADCAST, WM_SETTINGCHANGE, 0, (LPARAM)L"Environment",
        SMTO_ABORTIFHUNG, 5000, NULL);
    
    return true;
}

// Create uninstaller
void CreateUninstaller(const std::wstring& installPath) {
    std::wstring uninstallPath = installPath + L"\\uninstall.bat";
    std::wofstream uninstall(uninstallPath.c_str());
    
    uninstall << L"@echo off\n";
    uninstall << L"echo Uninstalling Flow...\n";
    uninstall << L"cd /d \"" << installPath << "\"\n";
    uninstall << L"del /q flow.exe\n";
    uninstall << L"del /q uninstall.bat\n";
    uninstall << L"cd ..\n";
    uninstall << L"rmdir /s /q Flow\n";
    uninstall << L"echo Flow has been uninstalled.\n";
    uninstall << L"pause\n";
    
    uninstall.close();
    
    // Add to Programs and Features
    HKEY hKey;
    if (RegCreateKeyExW(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\Flow",
        0, NULL, 0, KEY_WRITE, NULL, &hKey, NULL) == ERROR_SUCCESS) {
        
        std::wstring displayName = L"Flow Programming Language";
        std::wstring version = L"2.0.0";
        std::wstring publisher = L"AnonymoDGH";
        std::wstring uninstallString = installPath + L"\\uninstall.bat";
        
        RegSetValueExW(hKey, L"DisplayName", 0, REG_SZ, (LPBYTE)displayName.c_str(),
            (displayName.length() + 1) * sizeof(wchar_t));
        RegSetValueExW(hKey, L"DisplayVersion", 0, REG_SZ, (LPBYTE)version.c_str(),
            (version.length() + 1) * sizeof(wchar_t));
        RegSetValueExW(hKey, L"Publisher", 0, REG_SZ, (LPBYTE)publisher.c_str(),
            (publisher.length() + 1) * sizeof(wchar_t));
        RegSetValueExW(hKey, L"UninstallString", 0, REG_SZ, (LPBYTE)uninstallString.c_str(),
            (uninstallString.length() + 1) * sizeof(wchar_t));
        
        RegCloseKey(hKey);
    }
}

// Update status label
void UpdateStatus(const std::wstring& status) {
    SetWindowTextW(hStatusLabel, status.c_str());
    UpdateWindow(hStatusLabel);
}

// Set progress bar value
void SetProgress(int value) {
    SendMessage(hProgressBar, PBM_SETPOS, value, 0);
    UpdateWindow(hProgressBar);
}

// Download file from URL
bool DownloadFile(const std::wstring& url, const std::wstring& outputPath) {
    bool success = false;
    
    // Parse URL
    URL_COMPONENTS urlComp = { 0 };
    urlComp.dwStructSize = sizeof(urlComp);
    wchar_t hostName[256] = { 0 };
    wchar_t urlPath[1024] = { 0 };
    urlComp.lpszHostName = hostName;
    urlComp.dwHostNameLength = sizeof(hostName) / sizeof(wchar_t);
    urlComp.lpszUrlPath = urlPath;
    urlComp.dwUrlPathLength = sizeof(urlPath) / sizeof(wchar_t);
    
    if (!WinHttpCrackUrl(url.c_str(), url.length(), 0, &urlComp)) {
        return false;
    }
    
    // Initialize WinHTTP
    HINTERNET hSession = WinHttpOpen(L"Flow Installer/1.0",
        WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
        WINHTTP_NO_PROXY_NAME,
        WINHTTP_NO_PROXY_BYPASS, 0);
    
    if (!hSession) return false;
    
    // Connect
    HINTERNET hConnect = WinHttpConnect(hSession, hostName, urlComp.nPort, 0);
    if (!hConnect) {
        WinHttpCloseHandle(hSession);
        return false;
    }
    
    // Open request
    DWORD flags = (urlComp.nScheme == INTERNET_SCHEME_HTTPS) ? WINHTTP_FLAG_SECURE : 0;
    HINTERNET hRequest = WinHttpOpenRequest(hConnect, L"GET", urlPath,
        NULL, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, flags);
    
    if (!hRequest) {
        WinHttpCloseHandle(hConnect);
        WinHttpCloseHandle(hSession);
        return false;
    }
    
    // Send request
    if (WinHttpSendRequest(hRequest, WINHTTP_NO_ADDITIONAL_HEADERS, 0,
        WINHTTP_NO_REQUEST_DATA, 0, 0, 0) &&
        WinHttpReceiveResponse(hRequest, NULL)) {
        
        // Read data
        std::vector<BYTE> buffer;
        DWORD bytesRead = 0;
        BYTE tempBuffer[4096];
        
        while (WinHttpReadData(hRequest, tempBuffer, sizeof(tempBuffer), &bytesRead) && bytesRead > 0) {
            buffer.insert(buffer.end(), tempBuffer, tempBuffer + bytesRead);
        }
        
        // Write to file
        std::ofstream outFile(outputPath.c_str(), std::ios::binary);
        if (outFile.is_open()) {
            outFile.write(reinterpret_cast<const char*>(buffer.data()), buffer.size());
            outFile.close();
            success = true;
        }
    }
    
    WinHttpCloseHandle(hRequest);
    WinHttpCloseHandle(hConnect);
    WinHttpCloseHandle(hSession);
    
    return success;
}

// Install Flow
void InstallFlow() {
    EnableWindow(hInstallBtn, FALSE);
    EnableWindow(hCancelBtn, FALSE);
    EnableWindow(hPathEdit, FALSE);
    EnableWindow(hBrowseBtn, FALSE);
    
    SetProgress(0);
    UpdateStatus(L"Starting installation...");
    Sleep(500);
    
    try {
        // Create installation directory
        SetProgress(10);
        UpdateStatus(L"Creating installation directory...");
        fs::create_directories(installPath);
        Sleep(300);
        
        // Clone repository from GitHub
        SetProgress(20);
        UpdateStatus(L"Cloning Flow repository from GitHub...");
        
        std::wstring tempDir = installPath + L"_temp";
        std::wstring gitCmd = L"git clone https://github.com/" + 
            std::wstring(GITHUB_USER) + L"/" + std::wstring(GITHUB_REPO) + L".git \"" + tempDir + L"\"";
        
        STARTUPINFOW si = { sizeof(si) };
        PROCESS_INFORMATION pi;
        si.dwFlags = STARTF_USESHOWWINDOW;
        si.wShowWindow = SW_HIDE;
        
        if (!CreateProcessW(NULL, const_cast<LPWSTR>(gitCmd.c_str()), NULL, NULL, FALSE,
            CREATE_NO_WINDOW, NULL, NULL, &si, &pi)) {
            MessageBoxW(hMainWnd,
                L"Failed to run git clone. Please ensure Git is installed and in your PATH.\n\n"
                L"Download Git from: https://git-scm.com/download/win",
                L"Error", MB_OK | MB_ICONERROR);
            EnableWindow(hInstallBtn, TRUE);
            EnableWindow(hCancelBtn, TRUE);
            EnableWindow(hPathEdit, TRUE);
            EnableWindow(hBrowseBtn, TRUE);
            return;
        }
        
        WaitForSingleObject(pi.hProcess, INFINITE);
        DWORD exitCode;
        GetExitCodeProcess(pi.hProcess, &exitCode);
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
        
        if (exitCode != 0 || !fs::exists(tempDir)) {
            MessageBoxW(hMainWnd,
                L"Failed to clone repository. Please check your internet connection.",
                L"Error", MB_OK | MB_ICONERROR);
            EnableWindow(hInstallBtn, TRUE);
            EnableWindow(hCancelBtn, TRUE);
            EnableWindow(hPathEdit, TRUE);
            EnableWindow(hBrowseBtn, TRUE);
            return;
        }
        Sleep(500);
        
        // Compile Flow
        SetProgress(40);
        UpdateStatus(L"Compiling Flow compiler...");
        
        std::wstring srcFile = tempDir + L"\\src\\flow.cpp";
        std::wstring destExe = installPath + L"\\flow.exe";
        std::wstring compileCmd = L"g++ -std=c++17 -O2 -o \"" + destExe + L"\" \"" + srcFile + 
            L"\" -static-libgcc -static-libstdc++";
        
        si = { sizeof(si) };
        si.dwFlags = STARTF_USESHOWWINDOW;
        si.wShowWindow = SW_HIDE;
        
        if (!CreateProcessW(NULL, const_cast<LPWSTR>(compileCmd.c_str()), NULL, NULL, FALSE,
            CREATE_NO_WINDOW, NULL, NULL, &si, &pi)) {
            MessageBoxW(hMainWnd,
                L"Failed to compile Flow. Please ensure g++ (MinGW) is installed and in your PATH.\n\n"
                L"Download MinGW from: https://www.mingw-w64.org/",
                L"Error", MB_OK | MB_ICONERROR);
            fs::remove_all(tempDir);
            EnableWindow(hInstallBtn, TRUE);
            EnableWindow(hCancelBtn, TRUE);
            EnableWindow(hPathEdit, TRUE);
            EnableWindow(hBrowseBtn, TRUE);
            return;
        }
        
        WaitForSingleObject(pi.hProcess, INFINITE);
        GetExitCodeProcess(pi.hProcess, &exitCode);
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
        
        if (exitCode != 0 || !fs::exists(destExe)) {
            MessageBoxW(hMainWnd,
                L"Compilation failed. Please check that g++ is properly installed.",
                L"Error", MB_OK | MB_ICONERROR);
            std::error_code ec;
            fs::remove_all(tempDir, ec);
            EnableWindow(hInstallBtn, TRUE);
            EnableWindow(hCancelBtn, TRUE);
            EnableWindow(hPathEdit, TRUE);
            EnableWindow(hBrowseBtn, TRUE);
            return;
        }
        Sleep(500);
        
        // Copy examples and documentation
        SetProgress(55);
        UpdateStatus(L"Copying examples and documentation...");
        
        std::wstring srcExamples = tempDir + L"\\examples";
        std::wstring destExamples = installPath + L"\\examples";
        if (fs::exists(srcExamples)) {
            fs::copy(srcExamples, destExamples, fs::copy_options::recursive);
        }
        
        std::wstring srcDocs = tempDir + L"\\docs";
        std::wstring destDocs = installPath + L"\\docs";
        if (fs::exists(srcDocs)) {
            fs::copy(srcDocs, destDocs, fs::copy_options::recursive);
        }
        
        // Copy README files
        const wchar_t* readmes[] = {
            L"README.md", L"README-es.md", L"README-zh.md", 
            L"README-ja.md", L"README-fr.md"
        };
        for (const auto& readme : readmes) {
            std::wstring src = tempDir + L"\\" + readme;
            std::wstring dest = installPath + L"\\" + readme;
            if (fs::exists(src)) {
                fs::copy_file(src, dest, fs::copy_options::overwrite_existing);
            }
        }
        
        // Clean up temp directory
        SetProgress(70);
        UpdateStatus(L"Cleaning up temporary files...");
        
        std::error_code ec;
        fs::remove_all(tempDir, ec);
        
        // If fs::remove_all fails, try using system command
        if (ec || fs::exists(tempDir)) {
            std::wstring delCmd = L"cmd.exe /c rmdir /s /q \"" + tempDir + L"\"";
            STARTUPINFOW si2 = { sizeof(si2) };
            PROCESS_INFORMATION pi2;
            si2.dwFlags = STARTF_USESHOWWINDOW;
            si2.wShowWindow = SW_HIDE;
            
            if (CreateProcessW(NULL, const_cast<LPWSTR>(delCmd.c_str()), NULL, NULL, FALSE,
                CREATE_NO_WINDOW, NULL, NULL, &si2, &pi2)) {
                WaitForSingleObject(pi2.hProcess, 5000);
                CloseHandle(pi2.hProcess);
                CloseHandle(pi2.hThread);
            }
        }
        Sleep(300);
        
        // Add to PATH
        SetProgress(60);
        UpdateStatus(L"Adding Flow to system PATH...");
        if (!AddToPath(installPath)) {
            MessageBoxW(hMainWnd, L"Failed to add Flow to PATH. You may need to add it manually.",
                L"Warning", MB_OK | MB_ICONWARNING);
        }
        Sleep(500);
        
        // Create uninstaller
        SetProgress(80);
        UpdateStatus(L"Creating uninstaller...");
        CreateUninstaller(installPath);
        Sleep(300);
        
        // Create Start Menu shortcut
        SetProgress(90);
        UpdateStatus(L"Creating shortcuts...");
        
        wchar_t startMenuPath[MAX_PATH];
        if (SHGetFolderPathW(NULL, CSIDL_COMMON_PROGRAMS, NULL, 0, startMenuPath) == S_OK) {
            std::wstring flowFolder = std::wstring(startMenuPath) + L"\\Flow";
            fs::create_directories(flowFolder);
            
            // Create shortcut (simplified - in production use IShellLink)
            std::wstring cmdPath = flowFolder + L"\\Flow Command Prompt.lnk";
            // Note: Full shortcut creation requires COM, simplified here
        }
        Sleep(300);
        
        // Complete
        SetProgress(100);
        UpdateStatus(L"Installation complete!");
        Sleep(500);
        
        MessageBoxW(hMainWnd,
            L"Flow has been successfully installed!\n\n"
            L"You can now use 'flow' command from any command prompt.\n"
            L"Please restart any open command prompts for PATH changes to take effect.",
            L"Installation Complete",
            MB_OK | MB_ICONINFORMATION);
        
        PostQuitMessage(0);
        
    } catch (const std::exception& e) {
        std::wstring error = L"Installation failed: ";
        error += std::wstring(e.what(), e.what() + strlen(e.what()));
        MessageBoxW(hMainWnd, error.c_str(), L"Error", MB_OK | MB_ICONERROR);
        
        EnableWindow(hInstallBtn, TRUE);
        EnableWindow(hCancelBtn, TRUE);
        EnableWindow(hPathEdit, TRUE);
        EnableWindow(hBrowseBtn, TRUE);
    }
}

// Browse for folder
void BrowseForFolder() {
    BROWSEINFOW bi = { 0 };
    bi.hwndOwner = hMainWnd;
    bi.lpszTitle = L"Select Installation Folder";
    bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;
    
    LPITEMIDLIST pidl = SHBrowseForFolderW(&bi);
    if (pidl != NULL) {
        wchar_t path[MAX_PATH];
        if (SHGetPathFromIDListW(pidl, path)) {
            installPath = std::wstring(path) + L"\\Flow";
            SetWindowTextW(hPathEdit, installPath.c_str());
        }
        CoTaskMemFree(pidl);
    }
}

// Window procedure
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_CREATE: {
        // Title
        CreateWindowW(L"STATIC", L"Flow Programming Language Installer",
            WS_CHILD | WS_VISIBLE | SS_CENTER,
            20, 20, 460, 30, hWnd, NULL, hInst, NULL);
        
        // Version
        CreateWindowW(L"STATIC", L"Version 2.0.0",
            WS_CHILD | WS_VISIBLE | SS_CENTER,
            20, 50, 460, 20, hWnd, NULL, hInst, NULL);
        
        // Installation path label
        CreateWindowW(L"STATIC", L"Installation Path:",
            WS_CHILD | WS_VISIBLE,
            20, 90, 460, 20, hWnd, NULL, hInst, NULL);
        
        // Installation path edit
        hPathEdit = CreateWindowW(L"EDIT", installPath.c_str(),
            WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
            20, 110, 380, 25, hWnd, NULL, hInst, NULL);
        
        // Browse button
        hBrowseBtn = CreateWindowW(L"BUTTON", L"Browse...",
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            410, 110, 70, 25, hWnd, (HMENU)3, hInst, NULL);
        
        // Progress bar
        hProgressBar = CreateWindowExW(0, PROGRESS_CLASSW, NULL,
            WS_CHILD | WS_VISIBLE,
            20, 160, 460, 25, hWnd, NULL, hInst, NULL);
        SendMessage(hProgressBar, PBM_SETRANGE, 0, MAKELPARAM(0, 100));
        
        // Status label
        hStatusLabel = CreateWindowW(L"STATIC", L"Ready to install",
            WS_CHILD | WS_VISIBLE,
            20, 195, 460, 20, hWnd, NULL, hInst, NULL);
        
        // Install button
        hInstallBtn = CreateWindowW(L"BUTTON", L"Install",
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            280, 230, 100, 30, hWnd, (HMENU)1, hInst, NULL);
        
        // Cancel button
        hCancelBtn = CreateWindowW(L"BUTTON", L"Cancel",
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            390, 230, 90, 30, hWnd, (HMENU)2, hInst, NULL);
        
        break;
    }
    
    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case 1: // Install button
            {
                wchar_t path[MAX_PATH];
                GetWindowTextW(hPathEdit, path, MAX_PATH);
                installPath = path;
                
                if (!IsAdmin()) {
                    MessageBoxW(hWnd,
                        L"This installer requires administrator privileges.\n"
                        L"Please run as administrator.",
                        L"Administrator Required",
                        MB_OK | MB_ICONWARNING);
                } else {
                    InstallFlow();
                }
            }
            break;
            
        case 2: // Cancel button
            PostQuitMessage(0);
            break;
            
        case 3: // Browse button
            BrowseForFolder();
            break;
        }
        break;
        
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
        
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// WinMain
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    hInst = hInstance;
    
    // Initialize common controls
    INITCOMMONCONTROLSEX icex;
    icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
    icex.dwICC = ICC_PROGRESS_CLASS;
    InitCommonControlsEx(&icex);
    
    // Register window class
    WNDCLASSEXW wcex = { 0 };
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.hInstance = hInstance;
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszClassName = L"FlowInstallerClass";
    
    RegisterClassExW(&wcex);
    
    // Create window
    hMainWnd = CreateWindowExW(
        0,
        L"FlowInstallerClass",
        L"Flow Installer",
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
        CW_USEDEFAULT, CW_USEDEFAULT, 520, 320,
        NULL, NULL, hInstance, NULL);
    
    if (!hMainWnd) {
        return FALSE;
    }
    
    ShowWindow(hMainWnd, nCmdShow);
    UpdateWindow(hMainWnd);
    
    // Message loop
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    
    return (int)msg.wParam;
}
