#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/TextureRenderTarget2D.h"
#include "DNNDetector/ObjectDetectorBase.h"
#include <memory>

#include "ObjectDetectorActor.generated.h"

UCLASS()
class OBJECTDETECTION_API AObjectDetectorActor : public AActor
{
    GENERATED_BODY()

public:
    AObjectDetectorActor();

protected:
    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
    virtual void Tick(float DeltaTime) override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        UTextureRenderTarget2D* TextureRenderTarget;

private:
    FRenderTarget* m_renderTarget;
    TArray<FColor> m_colorArray;

    int m_texRows;
    int m_texCols;
    uint8_t* m_bgrBytes;

    std::shared_ptr<ObjectDetectorBase> m_detector;
};
