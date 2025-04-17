#include <cstring>
#include <fstream>
#include <iostream>
#include <vector>

#include "hpatch.h"

int main(int argc, char* argv[]) {
  // 检查参数数量是否正确
  if (argc != 4) {
    std::cerr << "用法: " << argv[0] << " <旧文件> <补丁文件> <新文件>"
              << std::endl;
    return 1;
  }

  // 获取命令行参数
  const char* oldFile = argv[1];
  const char* patchFile = argv[2];
  const char* newFile = argv[3];

  // 应用补丁生成新文件
  if (hpatchz(oldFile, patchFile, newFile, -1) != 0) {
    std::cerr << "应用补丁失败" << std::endl;
    return 1;
  }

  std::cout << "差分和补丁操作成功完成。" << std::endl;
  return 0;
}
