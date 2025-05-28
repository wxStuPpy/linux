#include <GL/glut.h>
#include <iostream>

int main(int argc, char** argv) {
    // 初始化 GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    
    // 创建一个隐藏窗口（无需显示）
    glutInitWindowSize(100, 100);
    glutCreateWindow("OpenGL Version Query");
    
    // 获取 OpenGL 版本和渲染器信息
    const char* version = (const char*)glGetString(GL_VERSION);
    const char* renderer = (const char*)glGetString(GL_RENDERER);
    const char* vendor = (const char*)glGetString(GL_VENDOR);
    
    // 输出信息
    std::cout << "OpenGL Version: " << version << std::endl;
    std::cout << "Renderer: " << renderer << std::endl;
    std::cout << "Vendor: " << vendor << std::endl;
    
    // 无需进入主循环，直接退出
    return 0;
}