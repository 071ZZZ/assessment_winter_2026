#include <cstdio>
#include <sstream>
#include <iostream>

#include "rm_a0/a0_08_raii_handle.hpp"
namespace rm_a0{
  FileHandle::FileHandle(const char *path, const char *mode){
    fp_=fopen(path,mode);
  }
  FileHandle::~FileHandle(){
    if(fp_!=nullptr){
      fclose(fp_);
      fp_=nullptr;
    }
  }
  FileHandle::FileHandle(FileHandle &&other) noexcept : fp_(other.fp_){
    other.fp_=nullptr;
  }
  FileHandle&
  FileHandle::operator=(FileHandle &&other) noexcept{
    if(this!=&other){
      if(fp_!=nullptr){
        fclose(fp_);
      }
    fp_=other.fp_;
    other.fp_=nullptr;
    }
    return *this;
  }
  bool FileHandle::valid() const {
    return fp_ != nullptr;
  }
  FILE* FileHandle::get() const {
    return fp_;
  }
  bool CopyFile(const std::string& in_path, const std::string& out_path) {
    FileHandle in_file(in_path.c_str(), "r");
    FileHandle out_file(out_path.c_str(), "w");
    if (!in_file.valid() || !out_file.valid()) {
        return false;
    }
    char buf[4096];
    size_t n;
    while ((n = fread(buf, 1, sizeof(buf), in_file.get())) > 0) {
        if (fwrite(buf, 1, n, out_file.get()) != n) {
            return false;
        }
    }
    return !ferror(in_file.get());  
  }
  std::string SolveRaiiCopy(const std::string& input, bool& ok) {
    std::istringstream in(input);
    std::string in_path;
    std::string out_path;
    if (!(in >> in_path >> out_path)) {
        ok = false;
        return {};
    }

    if (CopyFile(in_path, out_path)) {
        ok = true;
        return "OK\n";
    }

    ok = false;
    return "FAIL\n";
  }
}


int main() {
  std::string in_path, out_path;
  if (!(std::cin >> in_path >> out_path))
    return 0;

  if (rm_a0::CopyFile(in_path, out_path)) {
    std::cout << "OK\n";
  } else {
    std::cout << "FAIL\n";
  }

  return 0;
}
