下述操作请按顺序执行，打乱顺序可能导致异常。

【生产准备】
1. Windows 下安装 LPCXpresso，并申请授权文件、激活（否则烧写文件大小受限），假设安装目录为：X:\nxp\，请确保文件 X:\nxp\LPCXpresso_7.1.1_125\lpcxpresso\bin\crt_emu_lpc11_13_nxp.exe 存在

2. 修改 config.ini 文件
设置 lpcxpresso 路径为上述安装路径的 X:\nxp\LPCXpresso_7.1.1_125\lpcxpresso
【注意】请使用双反斜线，或单斜线，如：
X:\\nxp\\LPCXpresso_7.1.1_125\\lpcxpresso
X:/nxp/LPCXpresso_7.1.1_125/lpcxpresso
『注』如果 config.ini 文件不存在，可先运行一次 exe 并退出，将会自动生成

3. 连接好 LPC-Link 并运行 exe

4. Driver 的选择：默认安装使用hid，如果安装了 WinUSB 驱动，可以使用 winusb
5. Image 文件的选择：选择要烧写的文件

6. 测试 LPC-Link 的连接
取消“Burn Multiple Targets Automatically”选择框，程序进入“单步烧写状态”
单击 “Boot LPC” 按键，命令执行结束后，点击“Check LPC-Link”（建议等待十秒左右再点击，否则可能失败），可反复点击，直到成功为止。
【注意】如果 LPC-Link 未连接好（或被拔出后又重新插上），需重复上述步骤，先“Boot LPC”，然后“Check LPC-Link”，直到成功为止

7. 测试目标板连接
确保 LPC-Link 与目标板（待生产的硬件板）连接好，点击 “Check Target” 按键，直到检测到目标板成功为止，如果失败，请重新插拔目标板。如果插拔过程中导致 LPC-Link 与电脑断开，请重复步骤6

8. 测试烧写
点击 “Burn Image”，向目标板烧写文件，烧写后会提示成功或失败
【注意】不可执行的文件也可以烧写，但烧写后会提示失败


【批量生产】
1. 确保“生产准备”工作正确无误后，请勿修改配置
2. 勾选 “Burn Multiple Targets Automatically”选择框，然后点击“Run”，程序进入“连续烧写状态”
3. 更换新的目标板，注意该过程中不要断开 LPC-Link 与电脑的连接，断开 LPC-Link 与目标板的连接即可
4. 注意观察烧写后成功或失败，若烧写成功，拔下目标板，更换上新的目标板即可；若烧写失败，将会自动退回到单步状态
重复更换目标板，如果没有失败，程序将自动依次烧写目标板

