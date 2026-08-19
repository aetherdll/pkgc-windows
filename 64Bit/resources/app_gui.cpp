#include "app_gui.h"
#include "pkg.h"
#include "imgui.h"
#include <iostream>
#include <string>

struct CompilerForm {
    char packageName[128] = "MyPackage";
    int bitFormat = 1; // 0: 32 Bit, 1: 64 Bit
    char folderName[128] = "AssetsFolder";
    char savePath[256] = "C:/Package Compiler/";
    std::string statusMessage = "";
    bool isError = false;
};

static CompilerForm formData;

void ApplyCustomTheme() {
    ImGuiStyle& style = ImGui::GetStyle();
    ImVec4* colors = style.Colors;

    // Gri Arka Plan
    colors[ImGuiCol_WindowBg]         = ImVec4(0.16f, 0.16f, 0.16f, 1.00f);
    colors[ImGuiCol_FrameBg]          = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
    colors[ImGuiCol_FrameBgHovered]   = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
    colors[ImGuiCol_FrameBgActive]    = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);
    colors[ImGuiCol_TitleBg]          = ImVec4(0.12f, 0.12f, 0.12f, 1.00f);
    colors[ImGuiCol_TitleBgActive]    = ImVec4(0.12f, 0.12f, 0.12f, 1.00f);

    // Beyaz Yazı
    colors[ImGuiCol_Text]             = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);

    // Kırmızı Çizgiler ve Butonlar
    colors[ImGuiCol_Separator]        = ImVec4(0.85f, 0.10f, 0.10f, 1.00f);
    colors[ImGuiCol_CheckMark]        = ImVec4(0.90f, 0.15f, 0.15f, 1.00f);
    colors[ImGuiCol_Button]           = ImVec4(0.80f, 0.15f, 0.15f, 1.00f);
    colors[ImGuiCol_ButtonHovered]    = ImVec4(0.95f, 0.25f, 0.25f, 1.00f);
    colors[ImGuiCol_ButtonActive]     = ImVec4(0.65f, 0.05f, 0.05f, 1.00f);

    style.WindowRounding = 6.0f;
    style.FrameRounding  = 4.0f;
}

void RenderPackageCompilerWindow() {
    ImGui::SetNextWindowSize(ImVec2(500, 360), ImGuiCond_FirstUseEver);
    
    ImGui::Begin("Package Compiler (PKGC)", nullptr, ImGuiWindowFlags_NoCollapse);

    // 1. Paket Adı
    ImGui::Separator();
    ImGui::Text("Paket Adı Girin:");
    ImGui::SetNextItemWidth(-1);
    ImGui::InputText("##packageName", formData.packageName, IM_ARRAYSIZE(formData.packageName));

    ImGui::Spacing();

    // 2. Bit Formatı
    ImGui::Separator();
    ImGui::Text("Bit Formatını Girin:");
    ImGui::RadioButton("32 Bit", &formData.bitFormat, 0); 
    ImGui::SameLine();
    ImGui::RadioButton("64 Bit", &formData.bitFormat, 1);

    ImGui::Spacing();

    // 3. Klasör Adı
    ImGui::Separator();
    ImGui::Text("Klasör Adını Girin:");
    ImGui::SetNextItemWidth(-1);
    ImGui::InputText("##folderName", formData.folderName, IM_ARRAYSIZE(formData.folderName));

    ImGui::Spacing();

    // 4. Dosya Yolu
    ImGui::Separator();
    ImGui::Text("Kaydedilecek Dosya Yolu:");
    ImGui::SetNextItemWidth(-1);
    ImGui::InputText("##savePath", formData.savePath, IM_ARRAYSIZE(formData.savePath));

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    // 5. Kaydet Butonu
    float buttonWidth = 140.0f;
    float windowWidth = ImGui::GetWindowSize().x;
    ImGui::SetCursorPosX((windowWidth - buttonWidth) * 0.5f);

    if (ImGui::Button("Kaydet", ImVec2(buttonWidth, 32))) {
        uint8_t arch = (formData.bitFormat == 0) ? 32 : 64;
        
        // Hedef dosya yolunu oluştur (Örn: C:/Package Compiler/MyPackage.pkg)
        std::string fullPath = std::string(formData.savePath);
        if (!fullPath.empty() && fullPath.back() != '/' && fullPath.back() != '\\') {
            fullPath += "/";
        }
        fullPath += std::string(formData.packageName) + ".pkg";

        // Örnek Binary Payload (Senin verdiğin koddaki sample payload)
        std::vector<uint8_t> samplePayload = { 0x48, 0x31, 0xC0, 0xC3 };

        // DERLEYİCİ MOTORUNU ÇAĞIR
        if (PKGManager::createPackage(fullPath, formData.folderName, arch, samplePayload)) {
            formData.statusMessage = "[BAŞARILI] Paket derlendi: " + fullPath;
            formData.isError = false;
        } else {
            formData.statusMessage = "[HATA] Paket oluşturulamadı! Yolu kontrol edin.";
            formData.isError = true;
        }
    }

    // Durum Bildirimi
    if (!formData.statusMessage.empty()) {
        ImGui::Spacing();
        if (formData.isError) {
            ImGui::TextColored(ImVec4(1.0f, 0.3f, 0.3f, 1.0f), "%s", formData.statusMessage.c_str());
        } else {
            ImGui::TextColored(ImVec4(0.3f, 1.0f, 0.3f, 1.0f), "%s", formData.statusMessage.c_str());
        }
    }

    ImGui::End();
}