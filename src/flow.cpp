#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <filesystem>
#include <regex>
#include <cstdlib>
#include <algorithm>
#include <chrono>
#include <ctime>

namespace fs = std::filesystem;

// ANSI Color codes
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define BOLD    "\033[1m"

struct Module {
    std::string name;
    std::string lang;
    bool installed = false;
};

struct CodeBlock {
    std::string lang;  // "py", "js", "cpp"
    std::string code;
    int order;
};

class FlowCompiler {
private:
    std::stringstream py, js, cpp, pyCleanup;
    std::map<std::string, std::string> vars;
    std::set<std::string> pyImports, jsImports, cppIncludes;
    std::set<std::string> jsFunctions;
    std::vector<Module> modules;
    std::vector<CodeBlock> blocks;  // Para modo bidireccional
    bool asyncMode = false;
    bool inCleanupMode = false;
    bool failFast = true;
    bool useMemorySharing = true;
    bool bidirectionalMode = false;
    bool parallelMode = false;
    int currentBlockOrder = 0;

public:
    void registerJSFunc(const std::string& name) { jsFunctions.insert(name); }
    bool isJSFunc(const std::string& name) { return jsFunctions.count(name) > 0; }
    
    void import(const std::string& pkg, const std::string& lang = "auto") {
        if (lang == "py" || lang == "auto") pyImports.insert(pkg);
        if (lang == "js" || lang == "auto") jsImports.insert(pkg);
        if (lang == "cpp") cppIncludes.insert(pkg);
    }

    void addPy(const std::string& code) { 
        if (inCleanupMode) pyCleanup << code << "\n";
        else py << code << "\n"; 
    }
    void setCleanupMode(bool mode) { inCleanupMode = mode; }
    void addJS(const std::string& code) { js << code << "\n"; }
    void addCPP(const std::string& code) { cpp << code << "\n"; }

    void compile() {
        // Python with error handling
        std::ofstream pyf("__flow__.py");
        pyf << "import sys\n";
        pyf << "import json\n";
        for (auto& imp : pyImports) pyf << "import " << imp << "\n";
        pyf << "\n# Shared memory via JSON\n";
        pyf << "__flow_data__ = {}\n";
        pyf << "def flow_set(key, value):\n";
        pyf << "    __flow_data__[key] = value\n";
        pyf << "    with open('__flow_mem__.json', 'w') as f:\n";
        pyf << "        json.dump(__flow_data__, f)\n\n";
        pyf << "def flow_get(key, default=None):\n";
        pyf << "    try:\n";
        pyf << "        with open('__flow_mem__.json', 'r') as f:\n";
        pyf << "            data = json.load(f)\n";
        pyf << "            return data.get(key, default)\n";
        pyf << "    except: return default\n\n";
        pyf << "try:\n";
        
        // Indent all Python code
        std::string pyCode = py.str();
        std::istringstream pyStream(pyCode);
        std::string line;
        while (std::getline(pyStream, line)) {
            pyf << "    " << line << "\n";
        }
        
        pyf << "    sys.exit(0)\n";
        pyf << "except Exception as e:\n";
        pyf << "    print(f'" << RED << "✗ Python Error:" << RESET << " {e}', file=sys.stderr)\n";
        pyf << "    import traceback\n";
        pyf << "    traceback.print_exc()\n";
        pyf << "    sys.exit(1)\n";
        pyf.close();

        // JavaScript with error handling
        std::ofstream jsf("__flow__.js");
        jsf << "const fs = require('fs');\n";
        for (auto& imp : jsImports) {
            if (imp != "fs") jsf << "const " << imp << " = require('" << imp << "');\n";
        }
        jsf << "\n// Shared memory via JSON\n";
        jsf << "function flowSet(key, value) {\n";
        jsf << "    let data = {};\n";
        jsf << "    try { data = JSON.parse(fs.readFileSync('__flow_mem__.json', 'utf8')); } catch(e) {}\n";
        jsf << "    data[key] = value;\n";
        jsf << "    fs.writeFileSync('__flow_mem__.json', JSON.stringify(data));\n";
        jsf << "}\n\n";
        jsf << "function flowGet(key, defaultValue = null) {\n";
        jsf << "    try {\n";
        jsf << "        const data = JSON.parse(fs.readFileSync('__flow_mem__.json', 'utf8'));\n";
        jsf << "        return data[key] !== undefined ? data[key] : defaultValue;\n";
        jsf << "    } catch(e) { return defaultValue; }\n";
        jsf << "}\n\n";
        
        if (asyncMode) {
            jsf << "(async () => {\n";
            jsf << "try {\n";
            jsf << js.str();
            jsf << "    process.exit(0);\n";
            jsf << "} catch(e) {\n";
            jsf << "    console.error('" << RED << "✗ JavaScript Error:" << RESET << "', e.message);\n";
            jsf << "    console.error(e.stack);\n";
            jsf << "    process.exit(1);\n";
            jsf << "}\n";
            jsf << "})();\n";
        } else {
            jsf << "try {\n";
            jsf << js.str();
            jsf << "    process.exit(0);\n";
            jsf << "} catch(e) {\n";
            jsf << "    console.error('" << RED << "✗ JavaScript Error:" << RESET << "', e.message);\n";
            jsf << "    console.error(e.stack);\n";
            jsf << "    process.exit(1);\n";
            jsf << "}\n";
        }
        jsf.close();

        // C++ with error handling
        if (!cpp.str().empty()) {
            std::ofstream cppf("__flow__.cpp");
            cppf << "#include <iostream>\n#include <vector>\n#include <string>\n#include <cmath>\n#include <fstream>\n";
            cppf << "#include <map>\n#include <sstream>\n";
            for (auto& inc : cppIncludes) cppf << "#include <" << inc << ">\n";
            cppf << "\n// Shared memory via JSON\n";
            cppf << "std::map<std::string, std::string> flowData;\n\n";
            cppf << "void flowSet(const std::string& key, const std::string& value) {\n";
            cppf << "    flowData[key] = value;\n";
            cppf << "    std::ofstream f(\"__flow_mem__.json\");\n";
            cppf << "    f << \"{\";\n";
            cppf << "    bool first = true;\n";
            cppf << "    for(auto& p : flowData) {\n";
            cppf << "        if(!first) f << \",\";\n";
            cppf << "        f << \"\\\"\" << p.first << \"\\\":\\\"\" << p.second << \"\\\"\";\n";
            cppf << "        first = false;\n";
            cppf << "    }\n";
            cppf << "    f << \"}\";\n";
            cppf << "}\n\n";
            cppf << "std::string flowGet(const std::string& key, const std::string& defaultValue = \"\") {\n";
            cppf << "    std::ifstream f(\"__flow_mem__.json\");\n";
            cppf << "    if(!f.is_open()) return defaultValue;\n";
            cppf << "    // Simple JSON parsing for demo\n";
            cppf << "    return defaultValue;\n";
            cppf << "}\n\n";
            cppf << "int main() {\n";
            cppf << "try {\n";
            cppf << cpp.str();
            cppf << "return 0;\n";
            cppf << "} catch(const std::exception& e) {\n";
            cppf << "    std::cerr << \"" << RED << "✗ C++ Error:" << RESET << " \" << e.what() << std::endl;\n";
            cppf << "    return 1;\n";
            cppf << "}\n";
            cppf << "}\n";
            cppf.close();
        }
    }

    void executeParallel() {
        std::cout << CYAN << "→" << RESET << " Parallel mode: Starting concurrent execution\n\n";
        
        // Crear archivos de código
        compile();
        
        // Ejecutar en paralelo usando threads del sistema
        std::cout << BLUE << "[Parallel]" << RESET << " Launching Python, JavaScript, and C++ concurrently...\n";
        
        #ifdef _WIN32
        // Windows: usar start para procesos en background
        std::string pyCmd = "start /B python __flow__.py > __py_out__.txt 2>&1";
        std::string jsCmd = "start /B node __flow__.js > __js_out__.txt 2>&1";
        std::string cppCmd = "start /B cmd /c \"g++ -o __flow_bin__ __flow__.cpp -std=c++17 > __cpp_compile__.txt 2>&1 && __flow_bin__.exe > __cpp_out__.txt 2>&1\"";
        #else
        // Linux/Mac: usar & para background
        std::string pyCmd = "python __flow__.py > __py_out__.txt 2>&1 &";
        std::string jsCmd = "node __flow__.js > __js_out__.txt 2>&1 &";
        std::string cppCmd = "g++ -o __flow_bin__ __flow__.cpp -std=c++17 && ./__flow_bin__ > __cpp_out__.txt 2>&1 &";
        #endif
        
        // Lanzar procesos
        if (!py.str().empty()) {
            system(pyCmd.c_str());
            std::cout << GREEN << "  ✓" << RESET << " Python started\n";
        }
        
        if (!js.str().empty()) {
            system(jsCmd.c_str());
            std::cout << GREEN << "  ✓" << RESET << " JavaScript started\n";
        }
        
        if (!cpp.str().empty()) {
            system(cppCmd.c_str());
            std::cout << GREEN << "  ✓" << RESET << " C++ started\n";
        }
        
        // Esperar a que terminen (polling simple)
        std::cout << YELLOW << "\n[Parallel]" << RESET << " Waiting for all processes to complete...\n";
        
        #ifdef _WIN32
        system("timeout /t 5 /nobreak > nul");
        #else
        system("sleep 5");
        #endif
        
        // Mostrar outputs
        std::cout << "\n" << BLUE << "[Python Output]" << RESET << "\n";
        system("type __py_out__.txt 2>nul || echo No output");
        
        std::cout << "\n" << BLUE << "[JavaScript Output]" << RESET << "\n";
        system("type __js_out__.txt 2>nul || echo No output");
        
        std::cout << "\n" << BLUE << "[C++ Output]" << RESET << "\n";
        system("type __cpp_out__.txt 2>nul || echo No output");
        
        // Cleanup outputs
        remove("__py_out__.txt");
        remove("__js_out__.txt");
        remove("__cpp_out__.txt");
        remove("__cpp_compile__.txt");
    }
    
    void execute() {
        // Si está en modo paralelo, ejecutar concurrentemente
        if (parallelMode) {
            executeParallel();
            return;
        }
        
        // Si está en modo bidireccional, ejecutar bloques en orden
        if (bidirectionalMode) {
            executeBlocks();
            return;
        }
        
        int exitCode = 0;
        
        // Python
        if (!py.str().empty()) {
            std::cout << BLUE << "[Python]" << RESET << " Executing...\n";
            auto start = std::chrono::high_resolution_clock::now();
            exitCode = system("python __flow__.py 2>&1");
            auto end = std::chrono::high_resolution_clock::now();
            double duration = std::chrono::duration<double>(end - start).count();
            
            exportMetrics("python", duration, exitCode);
            
            if (exitCode != 0 && failFast) {
                std::cerr << RED << "✗ Pipeline stopped: Python failed with exit code " << exitCode << RESET << "\n";
                exportJUnitXML("Flow Pipeline", false, duration, "Python stage failed");
                return;
            }
        }
        
        // JavaScript
        if (!js.str().empty()) {
            std::cout << BLUE << "[JavaScript]" << RESET << " Executing...\n";
            auto start = std::chrono::high_resolution_clock::now();
            exitCode = system("node __flow__.js 2>&1");
            auto end = std::chrono::high_resolution_clock::now();
            double duration = std::chrono::duration<double>(end - start).count();
            
            exportMetrics("javascript", duration, exitCode);
            
            if (exitCode != 0 && failFast) {
                std::cerr << RED << "✗ Pipeline stopped: JavaScript failed with exit code " << exitCode << RESET << "\n";
                exportJUnitXML("Flow Pipeline", false, duration, "JavaScript stage failed");
                return;
            }
        }
        
        // C++
        if (!cpp.str().empty()) {
            std::cout << BLUE << "[C++]" << RESET << " Compiling...\n";
            auto start = std::chrono::high_resolution_clock::now();
            exitCode = system("g++ -o __flow_bin__ __flow__.cpp -std=c++17 2>&1");
            if (exitCode != 0 && failFast) {
                std::cerr << RED << "✗ Pipeline stopped: C++ compilation failed" << RESET << "\n";
                exportJUnitXML("Flow Pipeline", false, 0, "C++ compilation failed");
                return;
            }
            
            std::cout << BLUE << "[C++]" << RESET << " Executing...\n";
            #ifdef _WIN32
            exitCode = system("__flow_bin__.exe 2>&1");
            #else
            exitCode = system("./__flow_bin__ 2>&1");
            #endif
            
            auto end = std::chrono::high_resolution_clock::now();
            double duration = std::chrono::duration<double>(end - start).count();
            
            exportMetrics("cpp", duration, exitCode);
            
            if (exitCode != 0 && failFast) {
                std::cerr << RED << "✗ Pipeline stopped: C++ execution failed" << RESET << "\n";
                exportJUnitXML("Flow Pipeline", false, duration, "C++ execution failed");
                return;
            }
        }
        
        // Si todo pasó, exportar éxito
        exportJUnitXML("Flow Pipeline", true, 0);
        
        // Cleanup
        if (!pyCleanup.str().empty()) {
            std::cout << BLUE << "[Cleanup]" << RESET << " Executing...\n";
            std::ofstream cleanupf("__cleanup__.py");
            cleanupf << "import sys\n";
            cleanupf << "import json\n";
            for (auto& imp : pyImports) cleanupf << "import " << imp << "\n";
            
            // Add flow_get/flow_set functions
            cleanupf << "\ndef flow_set(key, value):\n";
            cleanupf << "    try:\n";
            cleanupf << "        with open('__flow_mem__.json', 'r') as f:\n";
            cleanupf << "            data = json.load(f)\n";
            cleanupf << "    except: data = {}\n";
            cleanupf << "    data[key] = value\n";
            cleanupf << "    with open('__flow_mem__.json', 'w') as f:\n";
            cleanupf << "        json.dump(data, f)\n\n";
            cleanupf << "def flow_get(key, default=None):\n";
            cleanupf << "    try:\n";
            cleanupf << "        with open('__flow_mem__.json', 'r') as f:\n";
            cleanupf << "            data = json.load(f)\n";
            cleanupf << "            return data.get(key, default)\n";
            cleanupf << "    except: return default\n\n";
            
            cleanupf << "try:\n";
            
            std::string cleanupCode = pyCleanup.str();
            std::istringstream cleanupStream(cleanupCode);
            std::string line;
            while (std::getline(cleanupStream, line)) {
                cleanupf << "    " << line << "\n";
            }
            
            cleanupf << "except Exception as e:\n";
            cleanupf << "    print(f'Cleanup warning: {e}', file=sys.stderr)\n";
            cleanupf.close();
            system("python __cleanup__.py 2>&1");
        }
    }

    void clean() {
        remove("__flow__.py");
        remove("__flow__.js");
        remove("__flow__.cpp");
        remove("__cleanup__.py");
        remove("__flow_mem__.json");
        remove("__flow_bin__");
        remove("__flow_bin__.exe");
        // No eliminar métricas y JUnit para CI/CD
        // remove("__flow_metrics__.json");
        // remove("__flow_junit__.xml");
        // remove("__flow_xcom__.json");
    }
    
    void setFailFast(bool value) { failFast = value; }
    void setBidirectional(bool value) { bidirectionalMode = value; }
    void setParallel(bool value) { parallelMode = value; }
    
    // Exportar métricas para observabilidad
    void exportMetrics(const std::string& stage, double duration, int exitCode) {
        std::ofstream metrics("__flow_metrics__.json", std::ios::app);
        if (metrics.is_open()) {
            metrics << "{\"stage\":\"" << stage << "\",\"duration\":" << duration 
                   << ",\"exit_code\":" << exitCode << ",\"timestamp\":" << time(nullptr) << "}\n";
            metrics.close();
        }
    }
    
    // Exportar resultados en formato JUnit XML para CI/CD
    void exportJUnitXML(const std::string& testName, bool passed, double duration, const std::string& error = "") {
        std::ofstream junit("__flow_junit__.xml");
        junit << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
        junit << "<testsuite name=\"Flow Tests\" tests=\"1\" failures=\"" << (passed ? 0 : 1) << "\" time=\"" << duration << "\">\n";
        junit << "  <testcase name=\"" << testName << "\" time=\"" << duration << "\"";
        if (!passed) {
            junit << ">\n    <failure message=\"" << error << "\"/>\n  </testcase>\n";
        } else {
            junit << "/>\n";
        }
        junit << "</testsuite>\n";
        junit.close();
    }
    
    // Exportar datos para Airflow/Prefect
    void exportAirflowXCom(const std::string& key, const std::string& value) {
        std::ofstream xcom("__flow_xcom__.json");
        xcom << "{\"" << key << "\":\"" << value << "\"}\n";
        xcom.close();
    }
    
    void addBlock(const std::string& lang, const std::string& code) {
        if (bidirectionalMode) {
            blocks.push_back({lang, code, currentBlockOrder++});
        }
    }
    
    void executeBlocks() {
        if (!bidirectionalMode) return;
        
        std::cout << CYAN << "→" << RESET << " Bidirectional mode: Executing blocks in order\n\n";
        
        for (auto& block : blocks) {
            int exitCode = 0;
            
            if (block.lang == "py") {
                std::cout << BLUE << "[Python Block " << block.order << "]" << RESET << " Executing...\n";
                
                std::ofstream pyf("__flow_block__.py");
                pyf << "import sys\nimport json\n";
                for (auto& imp : pyImports) pyf << "import " << imp << "\n";
                
                // Add flow functions
                pyf << "\ndef flow_set(key, value):\n";
                pyf << "    try:\n";
                pyf << "        with open('__flow_mem__.json', 'r') as f:\n";
                pyf << "            data = json.load(f)\n";
                pyf << "    except: data = {}\n";
                pyf << "    data[key] = value\n";
                pyf << "    with open('__flow_mem__.json', 'w') as f:\n";
                pyf << "        json.dump(data, f)\n\n";
                pyf << "def flow_get(key, default=None):\n";
                pyf << "    try:\n";
                pyf << "        with open('__flow_mem__.json', 'r') as f:\n";
                pyf << "            data = json.load(f)\n";
                pyf << "            return data.get(key, default)\n";
                pyf << "    except: return default\n\n";
                
                pyf << "try:\n";
                std::istringstream stream(block.code);
                std::string line;
                while (std::getline(stream, line)) {
                    pyf << "    " << line << "\n";
                }
                pyf << "    sys.exit(0)\n";
                pyf << "except Exception as e:\n";
                pyf << "    print(f'" << RED << "✗ Python Error:" << RESET << " {e}', file=sys.stderr)\n";
                pyf << "    import traceback\n";
                pyf << "    traceback.print_exc()\n";
                pyf << "    sys.exit(1)\n";
                pyf.close();
                
                exitCode = system("python __flow_block__.py 2>&1");
                remove("__flow_block__.py");
                
            } else if (block.lang == "js") {
                std::cout << BLUE << "[JavaScript Block " << block.order << "]" << RESET << " Executing...\n";
                
                std::ofstream jsf("__flow_block__.js");
                jsf << "const fs = require('fs');\n";
                for (auto& imp : jsImports) {
                    if (imp != "fs") jsf << "const " << imp << " = require('" << imp << "');\n";
                }
                
                jsf << "\nfunction flowSet(key, value) {\n";
                jsf << "    let data = {};\n";
                jsf << "    try { data = JSON.parse(fs.readFileSync('__flow_mem__.json', 'utf8')); } catch(e) {}\n";
                jsf << "    data[key] = value;\n";
                jsf << "    fs.writeFileSync('__flow_mem__.json', JSON.stringify(data));\n";
                jsf << "}\n\n";
                jsf << "function flowGet(key, defaultValue = null) {\n";
                jsf << "    try {\n";
                jsf << "        const data = JSON.parse(fs.readFileSync('__flow_mem__.json', 'utf8'));\n";
                jsf << "        return data[key] !== undefined ? data[key] : defaultValue;\n";
                jsf << "    } catch(e) { return defaultValue; }\n";
                jsf << "}\n\n";
                
                if (asyncMode) {
                    jsf << "(async () => {\n";
                    jsf << "try {\n";
                    jsf << block.code << "\n";
                    jsf << "    process.exit(0);\n";
                    jsf << "} catch(e) {\n";
                    jsf << "    console.error('" << RED << "✗ JavaScript Error:" << RESET << "', e.message);\n";
                    jsf << "    console.error(e.stack);\n";
                    jsf << "    process.exit(1);\n";
                    jsf << "}\n";
                    jsf << "})();\n";
                } else {
                    jsf << "try {\n";
                    jsf << block.code << "\n";
                    jsf << "    process.exit(0);\n";
                    jsf << "} catch(e) {\n";
                    jsf << "    console.error('" << RED << "✗ JavaScript Error:" << RESET << "', e.message);\n";
                    jsf << "    console.error(e.stack);\n";
                    jsf << "    process.exit(1);\n";
                    jsf << "}\n";
                }
                jsf.close();
                
                exitCode = system("node __flow_block__.js 2>&1");
                remove("__flow_block__.js");
                
            } else if (block.lang == "cpp") {
                std::cout << BLUE << "[C++ Block " << block.order << "]" << RESET << " Compiling...\n";
                
                std::ofstream cppf("__flow_block__.cpp");
                cppf << "#include <iostream>\n#include <vector>\n#include <string>\n#include <cmath>\n#include <fstream>\n";
                cppf << "#include <map>\n#include <sstream>\n";
                for (auto& inc : cppIncludes) cppf << "#include <" << inc << ">\n";
                
                cppf << "\nstd::map<std::string, std::string> flowData;\n\n";
                cppf << "void flowSet(const std::string& key, const std::string& value) {\n";
                cppf << "    flowData[key] = value;\n";
                cppf << "    std::ofstream f(\"__flow_mem__.json\");\n";
                cppf << "    f << \"{\";\n";
                cppf << "    bool first = true;\n";
                cppf << "    for(auto& p : flowData) {\n";
                cppf << "        if(!first) f << \",\";\n";
                cppf << "        f << \"\\\"\" << p.first << \"\\\":\\\"\" << p.second << \"\\\"\";\n";
                cppf << "        first = false;\n";
                cppf << "    }\n";
                cppf << "    f << \"}\";\n";
                cppf << "}\n\n";
                cppf << "std::string flowGet(const std::string& key, const std::string& defaultValue = \"\") {\n";
                cppf << "    std::ifstream f(\"__flow_mem__.json\");\n";
                cppf << "    if(!f.is_open()) return defaultValue;\n";
                cppf << "    return defaultValue;\n";
                cppf << "}\n\n";
                
                cppf << "int main() {\n";
                cppf << "try {\n";
                cppf << block.code << "\n";
                cppf << "return 0;\n";
                cppf << "} catch(const std::exception& e) {\n";
                cppf << "    std::cerr << \"" << RED << "✗ C++ Error:" << RESET << " \" << e.what() << std::endl;\n";
                cppf << "    return 1;\n";
                cppf << "}\n";
                cppf << "}\n";
                cppf.close();
                
                exitCode = system("g++ -o __flow_block__ __flow_block__.cpp -std=c++17 2>&1");
                if (exitCode == 0) {
                    std::cout << BLUE << "[C++ Block " << block.order << "]" << RESET << " Executing...\n";
                    #ifdef _WIN32
                    exitCode = system("__flow_block__.exe 2>&1");
                    remove("__flow_block__.exe");
                    #else
                    exitCode = system("./__flow_block__ 2>&1");
                    remove("__flow_block__");
                    #endif
                }
                remove("__flow_block__.cpp");
            }
            
            if (exitCode != 0 && failFast) {
                std::cerr << RED << "✗ Pipeline stopped: Block " << block.order << " failed" << RESET << "\n";
                return;
            }
        }
    }

    void enableAsync() { asyncMode = true; }
};

class FlowParser {
private:
    std::vector<std::string> lines;
    FlowCompiler* compiler;
    int pos = 0;
    std::map<std::string, std::string> macros;

public:
    FlowParser(const std::string& src, FlowCompiler* c) : compiler(c) {
        std::stringstream ss(src);
        std::string line;
        while (std::getline(ss, line)) lines.push_back(line);
        setupMacros();
    }

    void setupMacros() {
        macros["@web"] = "import requests, json";
        macros["@data"] = "import pandas as pd, numpy as np";
        macros["@ml"] = "import torch, tensorflow as tf";
        macros["@api"] = "const express = require('express'); const app = express();";
    }

    void parse() {
        while (pos < lines.size()) {
            std::string line = trim(lines[pos]);
            if (line.empty()) { pos++; continue; }
            if (line[0] == '#' && !startsWith(line, "# CLEANUP") && !startsWith(line, "# --- ETAPA DE LIMPIEZA")) { 
                pos++; 
                continue; 
            }

            if (line[0] == '@') {
                if (line == "@bidirectional") {
                    compiler->setBidirectional(true);
                    std::cout << YELLOW << "→ Bidirectional mode enabled" << RESET << "\n";
                    pos++;
                    continue;
                }
                if (line == "@parallel") {
                    compiler->setParallel(true);
                    std::cout << YELLOW << "→ Parallel mode enabled" << RESET << "\n";
                    pos++;
                    continue;
                }
                handleMacro(line);
            }
            else if (startsWith(line, "use ")) handleUse(line);
            else if (startsWith(line, "cpp")) parseCPP();
            else if (startsWith(line, "fn ")) parseJSFunc();
            else if (startsWith(line, "async ")) { compiler->enableAsync(); parseAsync(); }
            else if (startsWith(line, "def ")) parsePyFunc();
            else if (startsWith(line, "class ")) parsePyClass();
            else if (startsWith(line, "import ") || startsWith(line, "from ")) { compiler->addPy(line); pos++; }
            else if (startsWith(line, "const ") || startsWith(line, "let ") || startsWith(line, "var ")) { compiler->addJS(line + ";"); pos++; }
            else if (startsWith(line, "try:")) parsePyTry();
            else if (startsWith(line, "try {")) parseJSTry();
            else if (line.find("\"\"\"") != std::string::npos) { 
                compiler->addPy(line);
                pos++;
                while (pos < lines.size() && lines[pos].find("\"\"\"") == std::string::npos) {
                    compiler->addPy(lines[pos]);
                    pos++;
                }
                if (pos < lines.size()) {
                    compiler->addPy(lines[pos]);
                    pos++;
                }
            }
            else if (startsWith(line, "if ") || startsWith(line, "for ") || startsWith(line, "while ") || startsWith(line, "with ")) {
                parsePyBlock();
            }
            else if (startsWith(trim(line), "# CLEANUP") || startsWith(trim(line), "# --- ETAPA DE LIMPIEZA")) {
                compiler->setCleanupMode(true);
                pos++;
            }
            else { 
                autoDetect(line); 
                pos++; 
            }
        }
    }

private:
    std::string trim(const std::string& s) {
        auto start = s.find_first_not_of(" \t\r\n");
        if (start == std::string::npos) return "";
        auto end = s.find_last_not_of(" \t\r\n");
        return s.substr(start, end - start + 1);
    }

    bool startsWith(const std::string& s, const std::string& p) {
        return s.size() >= p.size() && s.substr(0, p.size()) == p;
    }

    int getIndent(const std::string& line) {
        int c = 0;
        for (char ch : line) {
            if (ch == ' ') c++;
            else if (ch == '\t') c += 4;
            else break;
        }
        return c;
    }

    void handleMacro(const std::string& line) {
        std::string macro = line.substr(0, line.find(' '));
        if (macros.count(macro)) {
            std::string code = macros[macro];
            if (code.find("import") != std::string::npos) compiler->addPy(code);
            else if (code.find("require") != std::string::npos) compiler->addJS(code);
        }
        pos++;
    }

    void handleUse(const std::string& line) {
        std::regex useRegex("use\\s+(\\w+)(?:\\s+as\\s+(py|js|cpp))?");
        std::smatch match;
        if (std::regex_search(line, match, useRegex)) {
            std::string pkg = match[1];
            std::string lang = match.size() > 2 ? match[2].str() : "auto";
            compiler->import(pkg, lang);
        }
        pos++;
    }

    void parsePyFunc() {
        compiler->addPy(lines[pos]);
        pos++;
        if (pos >= lines.size()) return;
        
        int base = -1;
        if (pos < lines.size() && !trim(lines[pos]).empty()) {
            base = getIndent(lines[pos]);
        }
        
        while (pos < lines.size()) {
            std::string next = lines[pos];
            if (trim(next).empty()) { compiler->addPy(""); pos++; continue; }
            if (base == -1) base = getIndent(next);
            if (getIndent(next) < base) break;
            compiler->addPy(next);
            pos++;
        }
    }

    void parsePyClass() {
        compiler->addPy(lines[pos]);
        pos++;
        if (pos >= lines.size()) return;
        
        int base = -1;
        if (pos < lines.size() && !trim(lines[pos]).empty()) {
            base = getIndent(lines[pos]);
        }
        
        while (pos < lines.size()) {
            std::string next = lines[pos];
            if (trim(next).empty()) { compiler->addPy(""); pos++; continue; }
            if (base == -1) base = getIndent(next);
            if (getIndent(next) < base) break;
            compiler->addPy(next);
            pos++;
        }
    }

    void parseJSFunc() {
        std::string line = lines[pos];
        std::regex fnRegex("fn\\s+(\\w+)\\s*\\(([^)]*)\\)\\s*:\\s*(.*)");
        std::smatch match;
        
        if (std::regex_search(line, match, fnRegex)) {
            std::string name = match[1];
            std::string params = match[2];
            std::string body = match[3];
            
            compiler->registerJSFunc(name);
            
            if (!body.empty()) {
                compiler->addJS("function " + name + "(" + params + ") { " + body + "; }");
                pos++;
            } else {
                compiler->addJS("function " + name + "(" + params + ") {");
                pos++;
                
                if (pos < lines.size()) {
                    int base = getIndent(lines[pos]);
                    while (pos < lines.size()) {
                        std::string next = lines[pos];
                        if (trim(next).empty()) { pos++; continue; }
                        if (getIndent(next) < base) break;
                        compiler->addJS("  " + trim(next));
                        pos++;
                    }
                }
                compiler->addJS("}");
            }
        } else pos++;
    }

    void parseAsync() {
        std::string line = lines[pos];
        std::regex asyncRegex("async\\s+fn\\s+(\\w+)\\s*\\(([^)]*)\\)\\s*:\\s*(.*)");
        std::smatch match;
        
        if (std::regex_search(line, match, asyncRegex)) {
            std::string name = match[1];
            std::string params = match[2];
            std::string body = match[3];
            
            compiler->registerJSFunc(name);
            
            if (!body.empty()) {
                compiler->addJS("async function " + name + "(" + params + ") { " + body + "; }");
                pos++;
            } else {
                compiler->addJS("async function " + name + "(" + params + ") {");
                pos++;
                
                if (pos < lines.size()) {
                    int base = -1;
                    if (pos < lines.size() && !trim(lines[pos]).empty()) {
                        base = getIndent(lines[pos]);
                    }
                    while (pos < lines.size()) {
                        std::string next = lines[pos];
                        if (trim(next).empty()) { pos++; continue; }
                        if (base == -1) base = getIndent(next);
                        if (getIndent(next) < base) break;
                        
                        // Skip Python docstrings in JS functions
                        if (trim(next).find("\"\"\"") != std::string::npos) {
                            pos++;
                            while (pos < lines.size() && lines[pos].find("\"\"\"") == std::string::npos) pos++;
                            if (pos < lines.size()) pos++;
                            continue;
                        }
                        
                        compiler->addJS("  " + trim(next));
                        pos++;
                    }
                }
                compiler->addJS("}");
            }
        } else pos++;
    }

    void parseCPP() {
        pos++;
        while (pos < lines.size()) {
            std::string line = lines[pos];
            if (trim(line) == "end") { pos++; break; }
            if (!line.empty() && line[0] != '#') {
                compiler->addCPP("  " + line);
            }
            pos++;
        }
    }

    void parsePyTry() {
        compiler->addPy(lines[pos]);
        pos++;
        if (pos >= lines.size()) return;
        
        int base = -1;
        if (pos < lines.size() && !trim(lines[pos]).empty()) {
            base = getIndent(lines[pos]);
        }
        
        while (pos < lines.size()) {
            std::string next = lines[pos];
            if (startsWith(trim(next), "except") && pos > 0 && getIndent(next) == getIndent(lines[pos-1])) {
                compiler->addPy(next);
                pos++;
                continue;
            }
            if (trim(next).empty()) { compiler->addPy(""); pos++; continue; }
            if (base == -1) base = getIndent(next);
            if (getIndent(next) < base && !startsWith(trim(next), "except")) break;
            compiler->addPy(next);
            pos++;
        }
    }

    void parseJSTry() {
        compiler->addJS(lines[pos]);
        pos++;
        int braces = 1;
        while (pos < lines.size() && braces > 0) {
            std::string line = lines[pos];
            compiler->addJS(line);
            for (char c : line) {
                if (c == '{') braces++;
                if (c == '}') braces--;
            }
            pos++;
        }
    }

    void parsePyBlock() {
        compiler->addPy(lines[pos]);
        pos++;
        if (pos >= lines.size()) return;
        
        int base = -1;
        if (pos < lines.size() && !trim(lines[pos]).empty()) {
            base = getIndent(lines[pos]);
        }
        
        while (pos < lines.size()) {
            std::string next = lines[pos];
            if (trim(next).empty()) { compiler->addPy(""); pos++; continue; }
            if (base == -1) base = getIndent(next);
            if (getIndent(next) < base) break;
            compiler->addPy(next);
            pos++;
        }
    }

    void autoDetect(const std::string& line) {
        std::regex callRegex("(\\w+)\\s*\\(");
        std::smatch match;
        
        if (std::regex_search(line, match, callRegex)) {
            std::string funcName = match[1];
            if (compiler->isJSFunc(funcName)) {
                compiler->addJS(line + ";");
                return;
            }
        }
        
        if (line.find("print(") != std::string::npos ||
            line.find("len(") != std::string::npos ||
            line.find("range(") != std::string::npos ||
            line.find("import ") != std::string::npos ||
            line.find("pd.") != std::string::npos ||
            line.find("os.") != std::string::npos ||
            line.find("json.") != std::string::npos ||
            line.find("= {") != std::string::npos) {
            compiler->addPy(line);
        } else if (line.find("console.") != std::string::npos ||
                   line.find(".map(") != std::string::npos ||
                   line.find("=>") != std::string::npos ||
                   line.find("const ") != std::string::npos ||
                   line.find("require(") != std::string::npos ||
                   line.find("fs.") != std::string::npos) {
            compiler->addJS(line + ";");
        } else {
            compiler->addPy(line);
        }
    }
};

void initProject(const std::string& name) {
    std::string dir = name.empty() ? "flow_app" : name;
    
    std::cout << CYAN << "→" << RESET << " Creating Flow project: " << BOLD << dir << RESET << "\n";
    
    fs::create_directories(dir);
    fs::create_directories(dir + "/src");
    
    // Create main.fl
    std::ofstream main(dir + "/main.fl");
    main << "# " << dir << " - Flow Application\n\n";
    main << "@data\n\n";
    main << "def hello():\n";
    main << "    print(\"Hello from Python!\")\n";
    main << "    return {\"message\": \"Flow is running\"}\n\n";
    main << "hello()\n\n";
    main << "fn greet():\n";
    main << "    console.log(\"Hello from JavaScript!\");\n\n";
    main << "greet()\n";
    main.close();
    
    // Create flow.json
    std::ofstream flowJson(dir + "/flow.json");
    flowJson << "{\n";
    flowJson << "  \"name\": \"" << dir << "\",\n";
    flowJson << "  \"version\": \"1.0.0\",\n";
    flowJson << "  \"description\": \"A Flow application\",\n";
    flowJson << "  \"main\": \"main.fl\",\n";
    flowJson << "  \"scripts\": {\n";
    flowJson << "    \"start\": \"flow main.fl\",\n";
    flowJson << "    \"dev\": \"flow main.fl\",\n";
    flowJson << "    \"test\": \"flow test.fl\"\n";
    flowJson << "  },\n";
    flowJson << "  \"dependencies\": {\n";
    flowJson << "    \"python\": [],\n";
    flowJson << "    \"javascript\": []\n";
    flowJson << "  }\n";
    flowJson << "}\n";
    flowJson.close();
    
    // Create README
    std::ofstream readme(dir + "/README.md");
    readme << "# " << dir << "\n\n";
    readme << "A Flow application\n\n";
    readme << "## Getting Started\n\n";
    readme << "```bash\n";
    readme << "cd " << dir << "\n";
    readme << "flow install\n";
    readme << "flow run start\n";
    readme << "```\n";
    readme.close();
    
    // Create .gitignore
    std::ofstream gitignore(dir + "/.gitignore");
    gitignore << "node_modules/\n";
    gitignore << "__pycache__/\n";
    gitignore << "*.pyc\n";
    gitignore << "__flow__.*\n";
    gitignore << "__cleanup__.*\n";
    gitignore << "__flow_bin__*\n";
    gitignore.close();
    
    std::cout << GREEN << "✓" << RESET << " Created " << BOLD << dir << "/" << RESET << "\n";
    std::cout << "  " << CYAN << "→" << RESET << " main.fl\n";
    std::cout << "  " << CYAN << "→" << RESET << " flow.json\n";
    std::cout << "  " << CYAN << "→" << RESET << " README.md\n";
    std::cout << "  " << CYAN << "→" << RESET << " .gitignore\n";
    std::cout << "\n" << BOLD << "Next steps:" << RESET << "\n";
    std::cout << "  cd " << dir << "\n";
    std::cout << "  flow run start\n";
}

std::string loadFileWithImports(const std::string& file, std::set<std::string>& loaded) {
    if (loaded.count(file)) return "";
    loaded.insert(file);
    
    std::ifstream f(file);
    if (!f.is_open()) {
        std::cerr << RED << "✗" << RESET << " File not found: " << BOLD << file << RESET << "\n";
        return "";
    }
    
    std::stringstream result;
    std::string line;
    
    while (std::getline(f, line)) {
        // Check for import directive
        std::regex importRegex("^\\s*import\\s+\"([^\"]+\\.fl)\"");
        std::smatch match;
        
        if (std::regex_search(line, match, importRegex)) {
            std::string importFile = match[1].str();
            std::cout << CYAN << "  →" << RESET << " Importing " << importFile << "\n";
            result << loadFileWithImports(importFile, loaded);
        } else {
            result << line << "\n";
        }
    }
    
    return result.str();
}

void runFile(const std::string& file) {
    std::cout << CYAN << "→" << RESET << " Running " << BOLD << file << RESET << "\n";
    
    std::set<std::string> loaded;
    std::string code = loadFileWithImports(file, loaded);
    
    if (code.empty()) {
        std::cerr << RED << "✗" << RESET << " Failed to load file\n";
        return;
    }
    
    std::cout << "\n";
    
    FlowCompiler compiler;
    FlowParser parser(code, &compiler);
    parser.parse();
    compiler.compile();
    compiler.execute();
    compiler.clean();
    
    std::cout << "\n" << GREEN << "✓" << RESET << " Execution completed\n";
}

std::string findFile(const std::string& n) {
    if (n.find(".fl") != std::string::npos && fs::exists(n)) return n;
    if (fs::exists(n + ".fl")) return n + ".fl";
    if (fs::exists("src/" + n + ".fl")) return "src/" + n + ".fl";
    if (fs::exists("main.fl")) return "main.fl";
    return "";
}

void printVersion() {
    std::cout << BOLD << CYAN << "Flow v2.0" << RESET << " - Multi-language programming\n";
}

void help() {
    printVersion();
    std::cout << "\n" << BOLD << "USAGE:" << RESET << "\n";
    std::cout << "  " << GREEN << "flow <file.fl>" << RESET << "              Run a Flow file\n";
    std::cout << "  " << GREEN << "flow init [name]" << RESET << "           Create new project\n";
    std::cout << "  " << GREEN << "flow install <pkg>" << RESET << "        Install package\n";
    std::cout << "  " << GREEN << "flow install" << RESET << "               Install all dependencies\n";
    std::cout << "  " << GREEN << "flow uninstall <pkg>" << RESET << "      Uninstall package\n";
    std::cout << "  " << GREEN << "flow list" << RESET << "                  List installed packages\n";
    std::cout << "  " << GREEN << "flow metrics" << RESET << "               Show execution metrics\n";
    std::cout << "  " << GREEN << "flow run <script>" << RESET << "         Run script from flow.json\n";
    std::cout << "  " << GREEN << "flow version" << RESET << "               Show version\n";
    std::cout << "  " << GREEN << "flow --help" << RESET << "                Show this help\n";
    std::cout << "\n" << BOLD << "EXAMPLES:" << RESET << "\n";
    std::cout << "  flow main.fl\n";
    std::cout << "  flow install pandas\n";
    std::cout << "  flow install express --js\n";
    std::cout << "  flow run dev\n";
}

void installPackage(const std::string& pkg, const std::string& lang = "auto") {
    std::cout << CYAN << "→" << RESET << " Installing " << BOLD << pkg << RESET << "...\n";
    
    if (lang == "py" || lang == "auto") {
        std::cout << YELLOW << "  [Python]" << RESET << " pip install " << pkg << "\n";
        int result = system(("pip install " + pkg + " 2>&1").c_str());
        if (result == 0) {
            std::cout << GREEN << "✓" << RESET << " " << pkg << " installed (Python)\n";
        } else {
            std::cout << RED << "✗" << RESET << " Failed to install " << pkg << " (Python)\n";
        }
    }
    
    if (lang == "js" || lang == "auto") {
        std::cout << YELLOW << "  [JavaScript]" << RESET << " npm install " << pkg << "\n";
        int result = system(("npm install " + pkg + " 2>&1").c_str());
        if (result == 0) {
            std::cout << GREEN << "✓" << RESET << " " << pkg << " installed (JavaScript)\n";
        } else {
            std::cout << RED << "✗" << RESET << " Failed to install " << pkg << " (JavaScript)\n";
        }
    }
}

void uninstallPackage(const std::string& pkg, const std::string& lang = "auto") {
    std::cout << CYAN << "→" << RESET << " Uninstalling " << BOLD << pkg << RESET << "...\n";
    
    if (lang == "py" || lang == "auto") {
        std::cout << YELLOW << "  [Python]" << RESET << " pip uninstall " << pkg << " -y\n";
        system(("pip uninstall " + pkg + " -y 2>&1").c_str());
    }
    
    if (lang == "js" || lang == "auto") {
        std::cout << YELLOW << "  [JavaScript]" << RESET << " npm uninstall " << pkg << "\n";
        system(("npm uninstall " + pkg + " 2>&1").c_str());
    }
    
    std::cout << GREEN << "✓" << RESET << " " << pkg << " uninstalled\n";
}

void listPackages() {
    std::cout << BOLD << CYAN << "Installed Packages:" << RESET << "\n\n";
    
    std::cout << YELLOW << "[Python]" << RESET << "\n";
    system("pip list 2>&1");
    
    std::cout << "\n" << YELLOW << "[JavaScript]" << RESET << "\n";
    system("npm list --depth=0 2>&1");
}

void showMetrics() {
    std::cout << BOLD << CYAN << "Flow Metrics:" << RESET << "\n\n";
    
    std::ifstream metrics("__flow_metrics__.json");
    if (!metrics.is_open()) {
        std::cout << YELLOW << "No metrics found. Run a Flow file first." << RESET << "\n";
        return;
    }
    
    std::string line;
    double totalDuration = 0;
    
    std::cout << BOLD << "Stage Performance:" << RESET << "\n";
    std::cout << "─────────────────────────────────────\n";
    
    while (std::getline(metrics, line)) {
        // Parse JSON simple
        size_t stagePos = line.find("\"stage\":\"");
        size_t durationPos = line.find("\"duration\":");
        size_t exitPos = line.find("\"exit_code\":");
        
        if (stagePos != std::string::npos && durationPos != std::string::npos) {
            size_t stageStart = stagePos + 9;
            size_t stageEnd = line.find("\"", stageStart);
            std::string stage = line.substr(stageStart, stageEnd - stageStart);
            
            size_t durStart = durationPos + 11;
            size_t durEnd = line.find(",", durStart);
            std::string durationStr = line.substr(durStart, durEnd - durStart);
            double duration = std::stod(durationStr);
            
            size_t exitStart = exitPos + 12;
            size_t exitEnd = line.find(",", exitStart);
            std::string exitCodeStr = line.substr(exitStart, exitEnd - exitStart);
            int exitCode = std::stoi(exitCodeStr);
            
            totalDuration += duration;
            
            std::cout << "  " << BLUE << stage << RESET << ": " 
                     << duration << "s ";
            if (exitCode == 0) {
                std::cout << GREEN << "✓" << RESET;
            } else {
                std::cout << RED << "✗ (exit " << exitCode << ")" << RESET;
            }
            std::cout << "\n";
        }
    }
    
    std::cout << "─────────────────────────────────────\n";
    std::cout << BOLD << "Total: " << totalDuration << "s" << RESET << "\n";
    
    metrics.close();
}

void installAll() {
    std::cout << CYAN << "→" << RESET << " Installing all dependencies...\n";
    
    // Check for requirements.txt
    if (fs::exists("requirements.txt")) {
        std::cout << YELLOW << "  [Python]" << RESET << " pip install -r requirements.txt\n";
        system("pip install -r requirements.txt 2>&1");
    }
    
    // Check for package.json
    if (fs::exists("package.json")) {
        std::cout << YELLOW << "  [JavaScript]" << RESET << " npm install\n";
        system("npm install 2>&1");
    }
    
    std::cout << GREEN << "✓" << RESET << " All dependencies installed\n";
}

void runScript(const std::string& scriptName) {
    std::ifstream flowJson("flow.json");
    if (!flowJson.is_open()) {
        std::cerr << RED << "✗" << RESET << " flow.json not found\n";
        return;
    }
    
    std::cout << CYAN << "→" << RESET << " Running script: " << BOLD << scriptName << RESET << "\n";
    
    // Simple JSON parsing for scripts
    std::string line;
    bool inScripts = false;
    std::string scriptCmd;
    
    while (std::getline(flowJson, line)) {
        if (line.find("\"scripts\"") != std::string::npos) {
            inScripts = true;
            continue;
        }
        if (inScripts && line.find("\"" + scriptName + "\"") != std::string::npos) {
            size_t start = line.find(":") + 1;
            size_t cmdStart = line.find("\"", start) + 1;
            size_t cmdEnd = line.find("\"", cmdStart);
            scriptCmd = line.substr(cmdStart, cmdEnd - cmdStart);
            break;
        }
    }
    
    if (scriptCmd.empty()) {
        std::cerr << RED << "✗" << RESET << " Script '" << scriptName << "' not found in flow.json\n";
        return;
    }
    
    // Replace "flow" with actual flow executable path if it's a flow command
    if (scriptCmd.find("flow ") == 0) {
        // Just run the file directly
        std::string file = scriptCmd.substr(5); // Remove "flow "
        std::cout << YELLOW << "  $" << RESET << " " << scriptCmd << "\n\n";
        runFile(file);
    } else {
        std::cout << YELLOW << "  $" << RESET << " " << scriptCmd << "\n";
        system(scriptCmd.c_str());
    }
}

int main(int argc, char** argv) {
    if (argc < 2) { help(); return 1; }
    
    std::string cmd = argv[1];
    
    // Commands
    if (cmd == "--help" || cmd == "-h") { 
        help(); 
        return 0; 
    }
    
    if (cmd == "version" || cmd == "-v" || cmd == "--version") { 
        printVersion(); 
        return 0; 
    }
    
    if (cmd == "init") { 
        initProject(argc > 2 ? argv[2] : ""); 
        return 0; 
    }
    
    if (cmd == "install") {
        if (argc < 3) {
            installAll();
        } else {
            std::string pkg = argv[2];
            std::string lang = "auto";
            
            // Check for language flag
            if (argc > 3) {
                std::string flag = argv[3];
                if (flag == "--py" || flag == "--python") lang = "py";
                else if (flag == "--js" || flag == "--javascript") lang = "js";
            }
            
            installPackage(pkg, lang);
        }
        return 0;
    }
    
    if (cmd == "uninstall" || cmd == "remove") {
        if (argc < 3) {
            std::cerr << RED << "✗" << RESET << " Package name required\n";
            return 1;
        }
        std::string pkg = argv[2];
        std::string lang = "auto";
        
        if (argc > 3) {
            std::string flag = argv[3];
            if (flag == "--py" || flag == "--python") lang = "py";
            else if (flag == "--js" || flag == "--javascript") lang = "js";
        }
        
        uninstallPackage(pkg, lang);
        return 0;
    }
    
    if (cmd == "list" || cmd == "ls") {
        listPackages();
        return 0;
    }
    
    if (cmd == "metrics") {
        showMetrics();
        return 0;
    }
    
    if (cmd == "run") {
        if (argc < 3) {
            std::cerr << RED << "✗" << RESET << " Script name required\n";
            return 1;
        }
        runScript(argv[2]);
        return 0;
    }
    
    // Try to run as file
    std::string file = findFile(cmd);
    if (file.empty()) { 
        std::cerr << RED << "✗" << RESET << " Command or file not found: " << BOLD << cmd << RESET << "\n";
        std::cerr << "Run " << GREEN << "flow --help" << RESET << " for usage\n";
        return 1; 
    }
    
    runFile(file);
    return 0;
}