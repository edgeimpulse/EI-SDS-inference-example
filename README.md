# Edge Impulse + SDS framework example
Examples running Edge Impulse SDK inference.
The data that is input to the model can be read from the onboard accelerometer and stored to the host (DebugRec and ReleaseRec config) or read from the host (DebugPlay and ReleasePlay config).

## Supported Boards
- Alif AppKit
- Alif DevKit
- ST B-U585I-IOT02A

## Update your model
> [!NOTE]
> This example only support accelerometer based model.

To update your model, unizp the CMSIS pack deployment, install the packs using cpackget add <your_project_name>.pack and paste the edgeimpulse.clayer.yml in the model folder.

## Build
Click the hammer Icon from the CMSIS tab view.

## Run
Click the Run or the Bug icon from the CMSIS tab view.
Remember to run the (sdsio-server)[https://github.com/ARM-software/SDS-Framework/blob/main/utilities/sdsio-server.py] script from the (SDS-Framework)[https://github.com/ARM-software/SDS-Framework] report before running the firwmare.
