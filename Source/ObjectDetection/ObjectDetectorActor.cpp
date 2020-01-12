#include "ObjectDetectorActor.h"
#include "DNNDetector/WebCamObjectDetector.h"

AObjectDetectorActor::AObjectDetectorActor()
{
    PrimaryActorTick.bCanEverTick = true;
}

void AObjectDetectorActor::BeginPlay()
{
    Super::BeginPlay();

    FString path = FPaths::ConvertRelativePathToFull(FPaths::ProjectDir()).Append("Binaries/Win64/");
    ObjectDetectorBase::Config config{ TCHAR_TO_UTF8(*path), "coco.names" , "yolov3.cfg" , "yolov3.weights" };
    m_detector = std::make_shared<ObjectDetectorBase>(config);

    m_renderTarget = TextureRenderTarget->GameThread_GetRenderTargetResource();
    m_texRows = TextureRenderTarget->SizeY;
    m_texCols = TextureRenderTarget->SizeX;
    m_bgrBytes = new uint8_t[m_texRows * m_texCols * 3];
}

void AObjectDetectorActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);

    delete[] m_bgrBytes;
}

void AObjectDetectorActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    m_renderTarget->ReadPixels(m_colorArray);

    for (int x = 0; x < m_texCols; ++x) {
        for (int y = 0; y < m_texRows; ++y) {
            const int k = x + y * m_texCols;
            const FColor PixelColor = m_colorArray[k];
            m_bgrBytes[k * 3] = PixelColor.B;
            m_bgrBytes[k * 3 + 1] = PixelColor.G;
            m_bgrBytes[k * 3 + 2] = PixelColor.R;
        }
    }

    cv::Mat frame(m_texRows, m_texCols, CV_8UC3, m_bgrBytes);
    m_detector->update(frame);
}
