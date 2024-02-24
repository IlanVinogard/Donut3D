#include <stdio.h>
#include <math.h>
#include <string.h>
#include <thread>

int main() {
    float rotationA = 0, rotationB = 0;
    float theta, phi;
    const int width = 80, height = 22;
    float depthBuffer[width * height];
    char outputBuffer[width * height];
    printf("\x1b[2J");
    while (1) {
        memset(outputBuffer, ' ', width * height);
        memset(depthBuffer, '\0', width * height * 4);
        for (phi = 0; phi < 6.28; phi += 0.02) {
            for (theta = 0; theta < 6.28; theta += 0.02) {
                //calculate Theta
                float sinTheta = sin(theta);
                //calculate Phi
                float sinPhi = sin(phi);
                float cosPhi = cos(phi);
                //calculate Rotation
                float sinRotationA = sin(rotationA);
                float cosRotationA = cos(rotationA);
                
                float circleOffset = cosPhi + 1.85;
                float inverseDepth = 1 / (sinTheta * circleOffset * sinRotationA + sinPhi * cosRotationA + 5);

                float cosTheta = cos(theta);
                float cosRotationB = cos(rotationB);
                float sinRotationB = sin(rotationB);
                float transformedX = sinTheta * circleOffset * cosRotationA - sinPhi * sinRotationA;
                
                int screenX = 60 + 30 * inverseDepth * (cosTheta * circleOffset * cosRotationB - transformedX * sinRotationB);
                int screenY = 12 + 15 * inverseDepth * (cosTheta * circleOffset * sinRotationB + transformedX * cosRotationB);

                int bufferIndex = screenX + 80 * screenY;
                int brightnessIndex = 8 * ((sinPhi * sinRotationA - sinTheta * cosPhi * cosRotationA) * cosRotationB - sinTheta * cosPhi * sinRotationA - sinPhi * cosRotationA - cosTheta * cosPhi * sinRotationB);

                if (22 > screenY && screenY > 0 && screenX > 0 && 80 > screenX && inverseDepth > depthBuffer[bufferIndex]) {
                    depthBuffer[bufferIndex] = inverseDepth;
                    outputBuffer[bufferIndex] = ".,-~:;=!*#$@"[brightnessIndex > 0 ? brightnessIndex : 0];
                }
            }
        }
        printf("\x1b[H");
        for (int k = 0; k <= width * height; k++) {
            putchar(k % 80 ? outputBuffer[k] : 10);
            rotationA += 0.00004;
            rotationB += 0.00002;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(70));
    }
    return 0;
}