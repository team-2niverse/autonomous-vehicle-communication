################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
"../Libraries/Ethernet/lwip/src/core/altcp.c" \
"../Libraries/Ethernet/lwip/src/core/altcp_alloc.c" \
"../Libraries/Ethernet/lwip/src/core/altcp_tcp.c" \
"../Libraries/Ethernet/lwip/src/core/def.c" \
"../Libraries/Ethernet/lwip/src/core/dns.c" \
"../Libraries/Ethernet/lwip/src/core/inet_chksum.c" \
"../Libraries/Ethernet/lwip/src/core/init.c" \
"../Libraries/Ethernet/lwip/src/core/ip.c" \
"../Libraries/Ethernet/lwip/src/core/mem.c" \
"../Libraries/Ethernet/lwip/src/core/memp.c" \
"../Libraries/Ethernet/lwip/src/core/netif.c" \
"../Libraries/Ethernet/lwip/src/core/pbuf.c" \
"../Libraries/Ethernet/lwip/src/core/raw.c" \
"../Libraries/Ethernet/lwip/src/core/stats.c" \
"../Libraries/Ethernet/lwip/src/core/sys.c" \
"../Libraries/Ethernet/lwip/src/core/tcp.c" \
"../Libraries/Ethernet/lwip/src/core/tcp_in.c" \
"../Libraries/Ethernet/lwip/src/core/tcp_out.c" \
"../Libraries/Ethernet/lwip/src/core/timeouts.c" \
"../Libraries/Ethernet/lwip/src/core/udp.c" 

COMPILED_SRCS += \
"Libraries/Ethernet/lwip/src/core/altcp.src" \
"Libraries/Ethernet/lwip/src/core/altcp_alloc.src" \
"Libraries/Ethernet/lwip/src/core/altcp_tcp.src" \
"Libraries/Ethernet/lwip/src/core/def.src" \
"Libraries/Ethernet/lwip/src/core/dns.src" \
"Libraries/Ethernet/lwip/src/core/inet_chksum.src" \
"Libraries/Ethernet/lwip/src/core/init.src" \
"Libraries/Ethernet/lwip/src/core/ip.src" \
"Libraries/Ethernet/lwip/src/core/mem.src" \
"Libraries/Ethernet/lwip/src/core/memp.src" \
"Libraries/Ethernet/lwip/src/core/netif.src" \
"Libraries/Ethernet/lwip/src/core/pbuf.src" \
"Libraries/Ethernet/lwip/src/core/raw.src" \
"Libraries/Ethernet/lwip/src/core/stats.src" \
"Libraries/Ethernet/lwip/src/core/sys.src" \
"Libraries/Ethernet/lwip/src/core/tcp.src" \
"Libraries/Ethernet/lwip/src/core/tcp_in.src" \
"Libraries/Ethernet/lwip/src/core/tcp_out.src" \
"Libraries/Ethernet/lwip/src/core/timeouts.src" \
"Libraries/Ethernet/lwip/src/core/udp.src" 

C_DEPS += \
"./Libraries/Ethernet/lwip/src/core/altcp.d" \
"./Libraries/Ethernet/lwip/src/core/altcp_alloc.d" \
"./Libraries/Ethernet/lwip/src/core/altcp_tcp.d" \
"./Libraries/Ethernet/lwip/src/core/def.d" \
"./Libraries/Ethernet/lwip/src/core/dns.d" \
"./Libraries/Ethernet/lwip/src/core/inet_chksum.d" \
"./Libraries/Ethernet/lwip/src/core/init.d" \
"./Libraries/Ethernet/lwip/src/core/ip.d" \
"./Libraries/Ethernet/lwip/src/core/mem.d" \
"./Libraries/Ethernet/lwip/src/core/memp.d" \
"./Libraries/Ethernet/lwip/src/core/netif.d" \
"./Libraries/Ethernet/lwip/src/core/pbuf.d" \
"./Libraries/Ethernet/lwip/src/core/raw.d" \
"./Libraries/Ethernet/lwip/src/core/stats.d" \
"./Libraries/Ethernet/lwip/src/core/sys.d" \
"./Libraries/Ethernet/lwip/src/core/tcp.d" \
"./Libraries/Ethernet/lwip/src/core/tcp_in.d" \
"./Libraries/Ethernet/lwip/src/core/tcp_out.d" \
"./Libraries/Ethernet/lwip/src/core/timeouts.d" \
"./Libraries/Ethernet/lwip/src/core/udp.d" 

OBJS += \
"Libraries/Ethernet/lwip/src/core/altcp.o" \
"Libraries/Ethernet/lwip/src/core/altcp_alloc.o" \
"Libraries/Ethernet/lwip/src/core/altcp_tcp.o" \
"Libraries/Ethernet/lwip/src/core/def.o" \
"Libraries/Ethernet/lwip/src/core/dns.o" \
"Libraries/Ethernet/lwip/src/core/inet_chksum.o" \
"Libraries/Ethernet/lwip/src/core/init.o" \
"Libraries/Ethernet/lwip/src/core/ip.o" \
"Libraries/Ethernet/lwip/src/core/mem.o" \
"Libraries/Ethernet/lwip/src/core/memp.o" \
"Libraries/Ethernet/lwip/src/core/netif.o" \
"Libraries/Ethernet/lwip/src/core/pbuf.o" \
"Libraries/Ethernet/lwip/src/core/raw.o" \
"Libraries/Ethernet/lwip/src/core/stats.o" \
"Libraries/Ethernet/lwip/src/core/sys.o" \
"Libraries/Ethernet/lwip/src/core/tcp.o" \
"Libraries/Ethernet/lwip/src/core/tcp_in.o" \
"Libraries/Ethernet/lwip/src/core/tcp_out.o" \
"Libraries/Ethernet/lwip/src/core/timeouts.o" \
"Libraries/Ethernet/lwip/src/core/udp.o" 


# Each subdirectory must supply rules for building sources it contributes
"Libraries/Ethernet/lwip/src/core/altcp.src":"../Libraries/Ethernet/lwip/src/core/altcp.c" "Libraries/Ethernet/lwip/src/core/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2004 -D__CPU__=tc37x "-fC:/Users/USER/Desktop/workspace_all/project2/autonomous-vehicle-FreeRTOS/iLLD_TC375_ADS_FreeRTOS_Basic/TriCore Debug (TASKING)/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc37x -Y0 -N0 -Z0 -o "$@" "$<"
"Libraries/Ethernet/lwip/src/core/altcp.o":"Libraries/Ethernet/lwip/src/core/altcp.src" "Libraries/Ethernet/lwip/src/core/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
"Libraries/Ethernet/lwip/src/core/altcp_alloc.src":"../Libraries/Ethernet/lwip/src/core/altcp_alloc.c" "Libraries/Ethernet/lwip/src/core/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2004 -D__CPU__=tc37x "-fC:/Users/USER/Desktop/workspace_all/project2/autonomous-vehicle-FreeRTOS/iLLD_TC375_ADS_FreeRTOS_Basic/TriCore Debug (TASKING)/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc37x -Y0 -N0 -Z0 -o "$@" "$<"
"Libraries/Ethernet/lwip/src/core/altcp_alloc.o":"Libraries/Ethernet/lwip/src/core/altcp_alloc.src" "Libraries/Ethernet/lwip/src/core/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
"Libraries/Ethernet/lwip/src/core/altcp_tcp.src":"../Libraries/Ethernet/lwip/src/core/altcp_tcp.c" "Libraries/Ethernet/lwip/src/core/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2004 -D__CPU__=tc37x "-fC:/Users/USER/Desktop/workspace_all/project2/autonomous-vehicle-FreeRTOS/iLLD_TC375_ADS_FreeRTOS_Basic/TriCore Debug (TASKING)/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc37x -Y0 -N0 -Z0 -o "$@" "$<"
"Libraries/Ethernet/lwip/src/core/altcp_tcp.o":"Libraries/Ethernet/lwip/src/core/altcp_tcp.src" "Libraries/Ethernet/lwip/src/core/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
"Libraries/Ethernet/lwip/src/core/def.src":"../Libraries/Ethernet/lwip/src/core/def.c" "Libraries/Ethernet/lwip/src/core/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2004 -D__CPU__=tc37x "-fC:/Users/USER/Desktop/workspace_all/project2/autonomous-vehicle-FreeRTOS/iLLD_TC375_ADS_FreeRTOS_Basic/TriCore Debug (TASKING)/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc37x -Y0 -N0 -Z0 -o "$@" "$<"
"Libraries/Ethernet/lwip/src/core/def.o":"Libraries/Ethernet/lwip/src/core/def.src" "Libraries/Ethernet/lwip/src/core/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
"Libraries/Ethernet/lwip/src/core/dns.src":"../Libraries/Ethernet/lwip/src/core/dns.c" "Libraries/Ethernet/lwip/src/core/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2004 -D__CPU__=tc37x "-fC:/Users/USER/Desktop/workspace_all/project2/autonomous-vehicle-FreeRTOS/iLLD_TC375_ADS_FreeRTOS_Basic/TriCore Debug (TASKING)/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc37x -Y0 -N0 -Z0 -o "$@" "$<"
"Libraries/Ethernet/lwip/src/core/dns.o":"Libraries/Ethernet/lwip/src/core/dns.src" "Libraries/Ethernet/lwip/src/core/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
"Libraries/Ethernet/lwip/src/core/inet_chksum.src":"../Libraries/Ethernet/lwip/src/core/inet_chksum.c" "Libraries/Ethernet/lwip/src/core/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2004 -D__CPU__=tc37x "-fC:/Users/USER/Desktop/workspace_all/project2/autonomous-vehicle-FreeRTOS/iLLD_TC375_ADS_FreeRTOS_Basic/TriCore Debug (TASKING)/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc37x -Y0 -N0 -Z0 -o "$@" "$<"
"Libraries/Ethernet/lwip/src/core/inet_chksum.o":"Libraries/Ethernet/lwip/src/core/inet_chksum.src" "Libraries/Ethernet/lwip/src/core/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
"Libraries/Ethernet/lwip/src/core/init.src":"../Libraries/Ethernet/lwip/src/core/init.c" "Libraries/Ethernet/lwip/src/core/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2004 -D__CPU__=tc37x "-fC:/Users/USER/Desktop/workspace_all/project2/autonomous-vehicle-FreeRTOS/iLLD_TC375_ADS_FreeRTOS_Basic/TriCore Debug (TASKING)/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc37x -Y0 -N0 -Z0 -o "$@" "$<"
"Libraries/Ethernet/lwip/src/core/init.o":"Libraries/Ethernet/lwip/src/core/init.src" "Libraries/Ethernet/lwip/src/core/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
"Libraries/Ethernet/lwip/src/core/ip.src":"../Libraries/Ethernet/lwip/src/core/ip.c" "Libraries/Ethernet/lwip/src/core/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2004 -D__CPU__=tc37x "-fC:/Users/USER/Desktop/workspace_all/project2/autonomous-vehicle-FreeRTOS/iLLD_TC375_ADS_FreeRTOS_Basic/TriCore Debug (TASKING)/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc37x -Y0 -N0 -Z0 -o "$@" "$<"
"Libraries/Ethernet/lwip/src/core/ip.o":"Libraries/Ethernet/lwip/src/core/ip.src" "Libraries/Ethernet/lwip/src/core/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
"Libraries/Ethernet/lwip/src/core/mem.src":"../Libraries/Ethernet/lwip/src/core/mem.c" "Libraries/Ethernet/lwip/src/core/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2004 -D__CPU__=tc37x "-fC:/Users/USER/Desktop/workspace_all/project2/autonomous-vehicle-FreeRTOS/iLLD_TC375_ADS_FreeRTOS_Basic/TriCore Debug (TASKING)/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc37x -Y0 -N0 -Z0 -o "$@" "$<"
"Libraries/Ethernet/lwip/src/core/mem.o":"Libraries/Ethernet/lwip/src/core/mem.src" "Libraries/Ethernet/lwip/src/core/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
"Libraries/Ethernet/lwip/src/core/memp.src":"../Libraries/Ethernet/lwip/src/core/memp.c" "Libraries/Ethernet/lwip/src/core/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2004 -D__CPU__=tc37x "-fC:/Users/USER/Desktop/workspace_all/project2/autonomous-vehicle-FreeRTOS/iLLD_TC375_ADS_FreeRTOS_Basic/TriCore Debug (TASKING)/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc37x -Y0 -N0 -Z0 -o "$@" "$<"
"Libraries/Ethernet/lwip/src/core/memp.o":"Libraries/Ethernet/lwip/src/core/memp.src" "Libraries/Ethernet/lwip/src/core/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
"Libraries/Ethernet/lwip/src/core/netif.src":"../Libraries/Ethernet/lwip/src/core/netif.c" "Libraries/Ethernet/lwip/src/core/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2004 -D__CPU__=tc37x "-fC:/Users/USER/Desktop/workspace_all/project2/autonomous-vehicle-FreeRTOS/iLLD_TC375_ADS_FreeRTOS_Basic/TriCore Debug (TASKING)/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc37x -Y0 -N0 -Z0 -o "$@" "$<"
"Libraries/Ethernet/lwip/src/core/netif.o":"Libraries/Ethernet/lwip/src/core/netif.src" "Libraries/Ethernet/lwip/src/core/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
"Libraries/Ethernet/lwip/src/core/pbuf.src":"../Libraries/Ethernet/lwip/src/core/pbuf.c" "Libraries/Ethernet/lwip/src/core/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2004 -D__CPU__=tc37x "-fC:/Users/USER/Desktop/workspace_all/project2/autonomous-vehicle-FreeRTOS/iLLD_TC375_ADS_FreeRTOS_Basic/TriCore Debug (TASKING)/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc37x -Y0 -N0 -Z0 -o "$@" "$<"
"Libraries/Ethernet/lwip/src/core/pbuf.o":"Libraries/Ethernet/lwip/src/core/pbuf.src" "Libraries/Ethernet/lwip/src/core/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
"Libraries/Ethernet/lwip/src/core/raw.src":"../Libraries/Ethernet/lwip/src/core/raw.c" "Libraries/Ethernet/lwip/src/core/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2004 -D__CPU__=tc37x "-fC:/Users/USER/Desktop/workspace_all/project2/autonomous-vehicle-FreeRTOS/iLLD_TC375_ADS_FreeRTOS_Basic/TriCore Debug (TASKING)/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc37x -Y0 -N0 -Z0 -o "$@" "$<"
"Libraries/Ethernet/lwip/src/core/raw.o":"Libraries/Ethernet/lwip/src/core/raw.src" "Libraries/Ethernet/lwip/src/core/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
"Libraries/Ethernet/lwip/src/core/stats.src":"../Libraries/Ethernet/lwip/src/core/stats.c" "Libraries/Ethernet/lwip/src/core/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2004 -D__CPU__=tc37x "-fC:/Users/USER/Desktop/workspace_all/project2/autonomous-vehicle-FreeRTOS/iLLD_TC375_ADS_FreeRTOS_Basic/TriCore Debug (TASKING)/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc37x -Y0 -N0 -Z0 -o "$@" "$<"
"Libraries/Ethernet/lwip/src/core/stats.o":"Libraries/Ethernet/lwip/src/core/stats.src" "Libraries/Ethernet/lwip/src/core/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
"Libraries/Ethernet/lwip/src/core/sys.src":"../Libraries/Ethernet/lwip/src/core/sys.c" "Libraries/Ethernet/lwip/src/core/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2004 -D__CPU__=tc37x "-fC:/Users/USER/Desktop/workspace_all/project2/autonomous-vehicle-FreeRTOS/iLLD_TC375_ADS_FreeRTOS_Basic/TriCore Debug (TASKING)/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc37x -Y0 -N0 -Z0 -o "$@" "$<"
"Libraries/Ethernet/lwip/src/core/sys.o":"Libraries/Ethernet/lwip/src/core/sys.src" "Libraries/Ethernet/lwip/src/core/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
"Libraries/Ethernet/lwip/src/core/tcp.src":"../Libraries/Ethernet/lwip/src/core/tcp.c" "Libraries/Ethernet/lwip/src/core/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2004 -D__CPU__=tc37x "-fC:/Users/USER/Desktop/workspace_all/project2/autonomous-vehicle-FreeRTOS/iLLD_TC375_ADS_FreeRTOS_Basic/TriCore Debug (TASKING)/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc37x -Y0 -N0 -Z0 -o "$@" "$<"
"Libraries/Ethernet/lwip/src/core/tcp.o":"Libraries/Ethernet/lwip/src/core/tcp.src" "Libraries/Ethernet/lwip/src/core/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
"Libraries/Ethernet/lwip/src/core/tcp_in.src":"../Libraries/Ethernet/lwip/src/core/tcp_in.c" "Libraries/Ethernet/lwip/src/core/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2004 -D__CPU__=tc37x "-fC:/Users/USER/Desktop/workspace_all/project2/autonomous-vehicle-FreeRTOS/iLLD_TC375_ADS_FreeRTOS_Basic/TriCore Debug (TASKING)/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc37x -Y0 -N0 -Z0 -o "$@" "$<"
"Libraries/Ethernet/lwip/src/core/tcp_in.o":"Libraries/Ethernet/lwip/src/core/tcp_in.src" "Libraries/Ethernet/lwip/src/core/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
"Libraries/Ethernet/lwip/src/core/tcp_out.src":"../Libraries/Ethernet/lwip/src/core/tcp_out.c" "Libraries/Ethernet/lwip/src/core/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2004 -D__CPU__=tc37x "-fC:/Users/USER/Desktop/workspace_all/project2/autonomous-vehicle-FreeRTOS/iLLD_TC375_ADS_FreeRTOS_Basic/TriCore Debug (TASKING)/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc37x -Y0 -N0 -Z0 -o "$@" "$<"
"Libraries/Ethernet/lwip/src/core/tcp_out.o":"Libraries/Ethernet/lwip/src/core/tcp_out.src" "Libraries/Ethernet/lwip/src/core/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
"Libraries/Ethernet/lwip/src/core/timeouts.src":"../Libraries/Ethernet/lwip/src/core/timeouts.c" "Libraries/Ethernet/lwip/src/core/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2004 -D__CPU__=tc37x -I"C:\\Users\\USER\\Desktop\\workspace_all\\project2\\autonomous-vehicle-FreeRTOS\\iLLD_TC375_ADS_FreeRTOS_Basic" -I"C:\\Users\\USER\\Desktop\\workspace_all\\project2\\autonomous-vehicle-FreeRTOS\\iLLD_TC375_ADS_FreeRTOS_Basic\\Configurations" -I"C:\\Users\\USER\\Desktop\\workspace_all\\project2\\autonomous-vehicle-FreeRTOS\\iLLD_TC375_ADS_FreeRTOS_Basic\\Libraries" -I"C:\\Users\\USER\\Desktop\\workspace_all\\project2\\autonomous-vehicle-FreeRTOS\\iLLD_TC375_ADS_FreeRTOS_Basic\\Libraries\\Ethernet" -I"C:\\Users\\USER\\Desktop\\workspace_all\\project2\\autonomous-vehicle-FreeRTOS\\iLLD_TC375_ADS_FreeRTOS_Basic\\Libraries\\Ethernet\\Phy_Dp83825i" -I"C:\\Users\\USER\\Desktop\\workspace_all\\project2\\autonomous-vehicle-FreeRTOS\\iLLD_TC375_ADS_FreeRTOS_Basic\\Libraries\\Ethernet\\lwip" -I"C:\\Users\\USER\\Desktop\\workspace_all\\project2\\autonomous-vehicle-FreeRTOS\\iLLD_TC375_ADS_FreeRTOS_Basic\\Libraries\\Ethernet\\lwip\\port" -I"C:\\Users\\USER\\Desktop\\workspace_all\\project2\\autonomous-vehicle-FreeRTOS\\iLLD_TC375_ADS_FreeRTOS_Basic\\Libraries\\Ethernet\\lwip\\port\\include" -I"C:\\Users\\USER\\Desktop\\workspace_all\\project2\\autonomous-vehicle-FreeRTOS\\iLLD_TC375_ADS_FreeRTOS_Basic\\Libraries\\Ethernet\\lwip\\src" -I"C:\\Users\\USER\\Desktop\\workspace_all\\project2\\autonomous-vehicle-FreeRTOS\\iLLD_TC375_ADS_FreeRTOS_Basic\\Libraries\\Ethernet\\lwip\\src\\include" -I"C:\\Users\\USER\\Desktop\\workspace_all\\project2\\autonomous-vehicle-FreeRTOS\\iLLD_TC375_ADS_FreeRTOS_Basic\\Libraries\\Ethernet\\lwip\\src\\include\\compat" -I"C:\\Users\\USER\\Desktop\\workspace_all\\project2\\autonomous-vehicle-FreeRTOS\\iLLD_TC375_ADS_FreeRTOS_Basic\\Libraries\\Ethernet\\lwip\\src\\include\\compat\\posix" -I"C:\\Users\\USER\\Desktop\\workspace_all\\project2\\autonomous-vehicle-FreeRTOS\\iLLD_TC375_ADS_FreeRTOS_Basic\\Libraries\\Ethernet\\lwip\\src\\include\\compat\\posix\\arpa" -I"C:\\Users\\USER\\Desktop\\workspace_all\\project2\\autonomous-vehicle-FreeRTOS\\iLLD_TC375_ADS_FreeRTOS_Basic\\Libraries\\Ethernet\\lwip\\src\\include\\compat\\posix\\net" -I"C:\\Users\\USER\\Desktop\\workspace_all\\project2\\autonomous-vehicle-FreeRTOS\\iLLD_TC375_ADS_FreeRTOS_Basic\\Libraries\\Ethernet\\lwip\\src\\include\\compat\\posix\\sys" -I"C:\\Users\\USER\\Desktop\\workspace_all\\project2\\autonomous-vehicle-FreeRTOS\\iLLD_TC375_ADS_FreeRTOS_Basic\\Libraries\\Ethernet\\lwip\\src\\include\\compat\\stdc" -I"C:\\Users\\USER\\Desktop\\workspace_all\\project2\\autonomous-vehicle-FreeRTOS\\iLLD_TC375_ADS_FreeRTOS_Basic\\Libraries\\Ethernet\\lwip\\src\\include\\lwip" -I"C:\\Users\\USER\\Desktop\\workspace_all\\project2\\autonomous-vehicle-FreeRTOS\\iLLD_TC375_ADS_FreeRTOS_Basic\\Libraries\\Ethernet\\lwip\\src\\include\\lwip\\apps" -I"C:\\Users\\USER\\Desktop\\workspace_all\\project2\\autonomous-vehicle-FreeRTOS\\iLLD_TC375_ADS_FreeRTOS_Basic\\Libraries\\Ethernet\\lwip\\src\\include\\lwip\\priv" -I"C:\\Users\\USER\\Desktop\\workspace_all\\project2\\autonomous-vehicle-FreeRTOS\\iLLD_TC375_ADS_FreeRTOS_Basic\\Libraries\\Ethernet\\lwip\\src\\include\\lwip\\prot" -I"C:\\Users\\USER\\Desktop\\workspace_all\\project2\\autonomous-vehicle-FreeRTOS\\iLLD_TC375_ADS_FreeRTOS_Basic\\Libraries\\Ethernet\\lwip\\src\\include\\netif" -I"C:\\Users\\USER\\Desktop\\workspace_all\\project2\\autonomous-vehicle-FreeRTOS\\iLLD_TC375_ADS_FreeRTOS_Basic\\Libraries\\Ethernet\\lwip\\src\\include\\netif\\ppp" -I"C:\\Users\\USER\\Desktop\\workspace_all\\project2\\autonomous-vehicle-FreeRTOS\\iLLD_TC375_ADS_FreeRTOS_Basic\\Libraries\\Ethernet\\lwip\\src\\include\\netif\\ppp\\polarssl" -I"C:\\Users\\USER\\Desktop\\workspace_all\\project2\\autonomous-vehicle-FreeRTOS\\iLLD_TC375_ADS_FreeRTOS_Basic\\Libraries\\Infra" -I"C:\\Users\\USER\\Desktop\\workspace_all\\project2\\autonomous-vehicle-FreeRTOS\\iLLD_TC375_ADS_FreeRTOS_Basic\\Libraries\\Infra\\Platform" -I"C:\\Users\\USER\\Desktop\\workspace_all\\project2\\autonomous-vehicle-FreeRTOS\\iLLD_TC375_ADS_FreeRTOS_Basic\\Libraries\\Infra\\Platform\\Tricore" -I"C:\\Users\\USER\\Desktop\\workspace_all\\project2\\autonomous-vehicle-FreeRTOS\\iLLD_TC375_ADS_FreeRTOS_Basic\\Libraries\\Infra\\Platform\\Tricore\\Compilers" -I"C:\\Users\\USER\\Desktop\\workspace_all\\project2\\autonomous-vehicle-FreeRTOS\\iLLD_TC375_ADS_FreeRTOS_Basic\\Libraries\\Infra\\Sfr" -I"C:\\Users\\USER\\Desktop\\workspace_all\\project2\\autonomous-vehicle-FreeRTOS\\iLLD_TC375_ADS_FreeRTOS_Basic\\Libraries\\Infra\\Sfr\\TC37A" -I"C:\\Users\\USER\\Desktop\\workspace_all\\project2\\autonomous-vehicle-FreeRTOS\\iLLD_TC375_ADS_FreeRTOS_Basic\\Libraries\\Infra\\Sfr\\TC37A\\_Reg" -I"C:\\Users\\USER\\Desktop\\workspace_all\\project2\\autonomous-vehicle-FreeRTOS\\iLLD_TC375_ADS_FreeRTOS_Basic\\Libraries\\Infra\\Ssw" -I"C:\\Users\\USER\\Desktop\\workspace_all\\project2\\autonomous-vehicle-FreeRTOS\\iLLD_TC375_ADS_FreeRTOS_Basic\\Libraries\\Infra\\Ssw\\TC37A" -I"C:\\Users\\USER\\Desktop\\workspace_all\\project2\\autonomous-vehicle-FreeRTOS\\iLLD_TC375_ADS_FreeRTOS_Basic\\Libraries\\Infra\\Ssw\\TC37A\\Tricore" -I"C:\\Users\\USER\\Desktop\\workspace_all\\project2\\autonomous-vehicle-FreeRTOS\\iLLD_TC375_ADS_FreeRTOS_Basic\\Libraries\\Service" -I"C:\\Users\\USER\\Desktop\\workspace_all\\project2\\autonomous-vehicle-FreeRTOS\\iLLD_TC375_ADS_FreeRTOS_Basic\\Libraries\\Service\\CpuGeneric" -I"C:\\Users\\USER\\Desktop\\workspace_all\\project2\\autonomous-vehicle-FreeRTOS\\iLLD_TC375_ADS_FreeRTOS_Basic\\Libraries\\Service\\CpuGeneric\\StdIf" -I"C:\\Users\\USER\\Desktop\\workspace_all\\project2\\autonomous-vehicle-FreeRTOS\\iLLD_TC375_ADS_FreeRTOS_Basic\\Libraries\\Service\\CpuGeneric\\SysSe" -I"C:\\Users\\USER\\Desktop\\workspace_all\\project2\\autonomous-vehicle-FreeRTOS\\iLLD_TC375_ADS_FreeRTOS_Basic\\Libraries\\Service\\CpuGeneric\\SysSe\\Comm" -I"C:\\Users\\USER\\Desktop\\workspace_all\\project2\\autonomous-vehicle-FreeRTOS\\iLLD_TC375_ADS_FreeRTOS_Basic\\Libraries\\Service\\CpuGeneric\\_Utilities" -I"C:\\Users\\USER\\Desktop\\workspace_all\\project2\\autonomous-vehicle-FreeRTOS\\iLLD_TC375_ADS_FreeRTOS_Basic\\Libraries\\iLLD" -I"C:\\Users\\USER\\Desktop\\workspace_all\\project2\\autonomous-vehicle-FreeRTOS\\iLLD_TC375_ADS_FreeRTOS_Basic\\Libraries\\iLLD\\TC37A" -I"C:\\Users\\USER\\Desktop\\workspace_all\\project2\\autonomous-vehicle-FreeRTOS\\iLLD_TC375_ADS_FreeRTOS_Basic\\Libraries\\iLLD\\TC37A\\Tricore" -I"C:\\Users\\USER\\Desktop\\workspace_all\\project2\\autonomous-vehicle-FreeRTOS\\iLLD_TC375_ADS_FreeRTOS_Basic\\Libraries\\iLLD\\TC37A\\Tricore\\Asclin" -I"C:\\Users\\USER\\Desktop\\workspace_all\\project2\\autonomous-vehicle-FreeRTOS\\iLLD_TC375_ADS_FreeRTOS_Basic\\Libraries\\iLLD\\TC37A\\Tricore\\Asclin\\Asc" -I"C:\\Users\\USER\\Desktop\\workspace_all\\project2\\autonomous-vehicle-FreeRTOS\\iLLD_TC375_ADS_FreeRTOS_Basic\\Libraries\\iLLD\\TC37A\\Tricore\\Asclin\\Std" -I"C:\\Users\\USER\\Desktop\\workspace_all\\project2\\autonomous-vehicle-FreeRTOS\\iLLD_TC375_ADS_FreeRTOS_Basic\\Libraries\\iLLD\\TC37A\\Tricore\\Cpu" -I"C:\\Users\\USER\\Desktop\\workspace_all\\project2\\autonomous-vehicle-FreeRTOS\\iLLD_TC375_ADS_FreeRTOS_Basic\\Libraries\\iLLD\\TC37A\\Tricore\\Cpu\\Irq" -I"C:\\Users\\USER\\Desktop\\workspace_all\\project2\\autonomous-vehicle-FreeRTOS\\iLLD_TC375_ADS_FreeRTOS_Basic\\Libraries\\iLLD\\TC37A\\Tricore\\Cpu\\Std" -I"C:\\Users\\USER\\Desktop\\workspace_all\\project2\\autonomous-vehicle-FreeRTOS\\iLLD_TC375_ADS_FreeRTOS_Basic\\Libraries\\iLLD\\TC37A\\Tricore\\Cpu\\Trap" -I"C:\\Users\\USER\\Desktop\\workspace_all\\project2\\autonomous-vehicle-FreeRTOS\\iLLD_TC375_ADS_FreeRTOS_Basic\\Libraries\\iLLD\\TC37A\\Tricore\\Geth" -I"C:\\Users\\USER\\Desktop\\workspace_all\\project2\\autonomous-vehicle-FreeRTOS\\iLLD_TC375_ADS_FreeRTOS_Basic\\Libraries\\iLLD\\TC37A\\Tricore\\Geth\\Eth" -I"C:\\Users\\USER\\Desktop\\workspace_all\\project2\\autonomous-vehicle-FreeRTOS\\iLLD_TC375_ADS_FreeRTOS_Basic\\Libraries\\iLLD\\TC37A\\Tricore\\Geth\\Std" -I"C:\\Users\\USER\\Desktop\\workspace_all\\project2\\autonomous-vehicle-FreeRTOS\\iLLD_TC375_ADS_FreeRTOS_Basic\\Libraries\\iLLD\\TC37A\\Tricore\\Gtm" -I"C:\\Users\\USER\\Desktop\\workspace_all\\project2\\autonomous-vehicle-FreeRTOS\\iLLD_TC375_ADS_FreeRTOS_Basic\\Libraries\\iLLD\\TC37A\\Tricore\\Gtm\\Std" -I"C:\\Users\\USER\\Desktop\\workspace_all\\project2\\autonomous-vehicle-FreeRTOS\\iLLD_TC375_ADS_FreeRTOS_Basic\\Libraries\\iLLD\\TC37A\\Tricore\\Mtu" -I"C:\\Users\\USER\\Desktop\\workspace_all\\project2\\autonomous-vehicle-FreeRTOS\\iLLD_TC375_ADS_FreeRTOS_Basic\\Libraries\\iLLD\\TC37A\\Tricore\\Mtu\\Std" -I"C:\\Users\\USER\\Desktop\\workspace_all\\project2\\autonomous-vehicle-FreeRTOS\\iLLD_TC375_ADS_FreeRTOS_Basic\\Libraries\\iLLD\\TC37A\\Tricore\\Pms" -I"C:\\Users\\USER\\Desktop\\workspace_all\\project2\\autonomous-vehicle-FreeRTOS\\iLLD_TC375_ADS_FreeRTOS_Basic\\Libraries\\iLLD\\TC37A\\Tricore\\Pms\\Std" -I"C:\\Users\\USER\\Desktop\\workspace_all\\project2\\autonomous-vehicle-FreeRTOS\\iLLD_TC375_ADS_FreeRTOS_Basic\\Libraries\\iLLD\\TC37A\\Tricore\\Port" -I"C:\\Users\\USER\\Desktop\\workspace_all\\project2\\autonomous-vehicle-FreeRTOS\\iLLD_TC375_ADS_FreeRTOS_Basic\\Libraries\\iLLD\\TC37A\\Tricore\\Port\\Std" -I"C:\\Users\\USER\\Desktop\\workspace_all\\project2\\autonomous-vehicle-FreeRTOS\\iLLD_TC375_ADS_FreeRTOS_Basic\\Libraries\\iLLD\\TC37A\\Tricore\\Scu" -I"C:\\Users\\USER\\Desktop\\workspace_all\\project2\\autonomous-vehicle-FreeRTOS\\iLLD_TC375_ADS_FreeRTOS_Basic\\Libraries\\iLLD\\TC37A\\Tricore\\Scu\\Std" -I"C:\\Users\\USER\\Desktop\\workspace_all\\project2\\autonomous-vehicle-FreeRTOS\\iLLD_TC375_ADS_FreeRTOS_Basic\\Libraries\\iLLD\\TC37A\\Tricore\\Src" -I"C:\\Users\\USER\\Desktop\\workspace_all\\project2\\autonomous-vehicle-FreeRTOS\\iLLD_TC375_ADS_FreeRTOS_Basic\\Libraries\\iLLD\\TC37A\\Tricore\\Src\\Std" -I"C:\\Users\\USER\\Desktop\\workspace_all\\project2\\autonomous-vehicle-FreeRTOS\\iLLD_TC375_ADS_FreeRTOS_Basic\\Libraries\\iLLD\\TC37A\\Tricore\\Stm" -I"C:\\Users\\USER\\Desktop\\workspace_all\\project2\\autonomous-vehicle-FreeRTOS\\iLLD_TC375_ADS_FreeRTOS_Basic\\Libraries\\iLLD\\TC37A\\Tricore\\Stm\\Std" -I"C:\\Users\\USER\\Desktop\\workspace_all\\project2\\autonomous-vehicle-FreeRTOS\\iLLD_TC375_ADS_FreeRTOS_Basic\\Libraries\\iLLD\\TC37A\\Tricore\\_Impl" -I"C:\\Users\\USER\\Desktop\\workspace_all\\project2\\autonomous-vehicle-FreeRTOS\\iLLD_TC375_ADS_FreeRTOS_Basic\\Libraries\\iLLD\\TC37A\\Tricore\\_Lib" -I"C:\\Users\\USER\\Desktop\\workspace_all\\project2\\autonomous-vehicle-FreeRTOS\\iLLD_TC375_ADS_FreeRTOS_Basic\\Libraries\\iLLD\\TC37A\\Tricore\\_Lib\\DataHandling" -I"C:\\Users\\USER\\Desktop\\workspace_all\\project2\\autonomous-vehicle-FreeRTOS\\iLLD_TC375_ADS_FreeRTOS_Basic\\Libraries\\iLLD\\TC37A\\Tricore\\_PinMap" -I"C:\\Users\\USER\\Desktop\\workspace_all\\project2\\autonomous-vehicle-FreeRTOS\\iLLD_TC375_ADS_FreeRTOS_Basic\\Libraries\\iLLD\\TC37A\\Tricore\\Port\\Io" -I"C:\\Users\\USER\\Desktop\\workspace_all\\project2\\autonomous-vehicle-FreeRTOS\\iLLD_TC375_ADS_FreeRTOS_Basic\\OS" -I"C:\\Users\\USER\\Desktop\\workspace_all\\project2\\autonomous-vehicle-FreeRTOS\\iLLD_TC375_ADS_FreeRTOS_Basic\\OS\\FreeRTOS" -I"C:\\Users\\USER\\Desktop\\workspace_all\\project2\\autonomous-vehicle-FreeRTOS\\iLLD_TC375_ADS_FreeRTOS_Basic\\OS\\FreeRTOS\\include" -I"C:\\Users\\USER\\Desktop\\workspace_all\\project2\\autonomous-vehicle-FreeRTOS\\iLLD_TC375_ADS_FreeRTOS_Basic\\OS\\FreeRTOS\\portable" -I"C:\\Users\\USER\\Desktop\\workspace_all\\project2\\autonomous-vehicle-FreeRTOS\\iLLD_TC375_ADS_FreeRTOS_Basic\\OS\\FreeRTOS\\portable\\Tasking" -I"C:\\Users\\USER\\Desktop\\workspace_all\\project2\\autonomous-vehicle-FreeRTOS\\iLLD_TC375_ADS_FreeRTOS_Basic\\OS\\FreeRTOS\\portable\\Tasking\\TC3" -I"C:\\Users\\USER\\Desktop\\workspace_all\\project2\\autonomous-vehicle-FreeRTOS\\iLLD_TC375_ADS_FreeRTOS_Basic\\Libraries\\iLLD\\TC37A\\Tricore\\Gtm\\Atom\\Pwm" -I"C:\\Users\\USER\\Desktop\\workspace_all\\project2\\autonomous-vehicle-FreeRTOS\\iLLD_TC375_ADS_FreeRTOS_Basic\\Libraries\\iLLD\\TC37A\\Tricore\\Can\\Can" -I"C:\\Users\\USER\\Desktop\\workspace_all\\project2\\autonomous-vehicle-FreeRTOS\\iLLD_TC375_ADS_FreeRTOS_Basic\\Libraries\\iLLD\\TC37A\\Tricore\\Can\\Std" -I"C:\\Users\\USER\\Desktop\\workspace_all\\project2\\autonomous-vehicle-FreeRTOS\\iLLD_TC375_ADS_FreeRTOS_Basic\\App" -I"C:\\Users\\USER\\Desktop\\workspace_all\\project2\\autonomous-vehicle-FreeRTOS\\iLLD_TC375_ADS_FreeRTOS_Basic\\App\\include" -I"C:\\Users\\USER\\Desktop\\workspace_all\\project2\\autonomous-vehicle-FreeRTOS\\iLLD_TC375_ADS_FreeRTOS_Basic\\App\\src" -I"C:\\Users\\USER\\Desktop\\workspace_all\\project2\\autonomous-vehicle-FreeRTOS\\iLLD_TC375_ADS_FreeRTOS_Basic\\Drivers" -I"C:\\Users\\USER\\Desktop\\workspace_all\\project2\\autonomous-vehicle-FreeRTOS\\iLLD_TC375_ADS_FreeRTOS_Basic\\Drivers\\include" -I"C:\\Users\\USER\\Desktop\\workspace_all\\project2\\autonomous-vehicle-FreeRTOS\\iLLD_TC375_ADS_FreeRTOS_Basic\\Drivers\\src" -I"C:\\Users\\USER\\Desktop\\workspace_all\\project2\\autonomous-vehicle-FreeRTOS\\iLLD_TC375_ADS_FreeRTOS_Basic\\Libraries\\lwip-freertos-integration-master" -I"C:\\Users\\USER\\Desktop\\workspace_all\\project2\\autonomous-vehicle-FreeRTOS\\iLLD_TC375_ADS_FreeRTOS_Basic\\Libraries\\lwip-freertos-integration-master\\arch" -I"C:\\Users\\USER\\Desktop\\workspace_all\\project2\\autonomous-vehicle-FreeRTOS\\iLLD_TC375_ADS_FreeRTOS_Basic\\Libraries\\Ethernet\\lwip\\port\\src" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc37x -Y0 -N0 -Z0 -o "$@" "$<"
"Libraries/Ethernet/lwip/src/core/timeouts.o":"Libraries/Ethernet/lwip/src/core/timeouts.src" "Libraries/Ethernet/lwip/src/core/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
"Libraries/Ethernet/lwip/src/core/udp.src":"../Libraries/Ethernet/lwip/src/core/udp.c" "Libraries/Ethernet/lwip/src/core/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2004 -D__CPU__=tc37x "-fC:/Users/USER/Desktop/workspace_all/project2/autonomous-vehicle-FreeRTOS/iLLD_TC375_ADS_FreeRTOS_Basic/TriCore Debug (TASKING)/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc37x -Y0 -N0 -Z0 -o "$@" "$<"
"Libraries/Ethernet/lwip/src/core/udp.o":"Libraries/Ethernet/lwip/src/core/udp.src" "Libraries/Ethernet/lwip/src/core/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"

clean: clean-Libraries-2f-Ethernet-2f-lwip-2f-src-2f-core

clean-Libraries-2f-Ethernet-2f-lwip-2f-src-2f-core:
	-$(RM) ./Libraries/Ethernet/lwip/src/core/altcp.d ./Libraries/Ethernet/lwip/src/core/altcp.o ./Libraries/Ethernet/lwip/src/core/altcp.src ./Libraries/Ethernet/lwip/src/core/altcp_alloc.d ./Libraries/Ethernet/lwip/src/core/altcp_alloc.o ./Libraries/Ethernet/lwip/src/core/altcp_alloc.src ./Libraries/Ethernet/lwip/src/core/altcp_tcp.d ./Libraries/Ethernet/lwip/src/core/altcp_tcp.o ./Libraries/Ethernet/lwip/src/core/altcp_tcp.src ./Libraries/Ethernet/lwip/src/core/def.d ./Libraries/Ethernet/lwip/src/core/def.o ./Libraries/Ethernet/lwip/src/core/def.src ./Libraries/Ethernet/lwip/src/core/dns.d ./Libraries/Ethernet/lwip/src/core/dns.o ./Libraries/Ethernet/lwip/src/core/dns.src ./Libraries/Ethernet/lwip/src/core/inet_chksum.d ./Libraries/Ethernet/lwip/src/core/inet_chksum.o ./Libraries/Ethernet/lwip/src/core/inet_chksum.src ./Libraries/Ethernet/lwip/src/core/init.d ./Libraries/Ethernet/lwip/src/core/init.o ./Libraries/Ethernet/lwip/src/core/init.src ./Libraries/Ethernet/lwip/src/core/ip.d ./Libraries/Ethernet/lwip/src/core/ip.o ./Libraries/Ethernet/lwip/src/core/ip.src ./Libraries/Ethernet/lwip/src/core/mem.d ./Libraries/Ethernet/lwip/src/core/mem.o ./Libraries/Ethernet/lwip/src/core/mem.src ./Libraries/Ethernet/lwip/src/core/memp.d ./Libraries/Ethernet/lwip/src/core/memp.o ./Libraries/Ethernet/lwip/src/core/memp.src ./Libraries/Ethernet/lwip/src/core/netif.d ./Libraries/Ethernet/lwip/src/core/netif.o ./Libraries/Ethernet/lwip/src/core/netif.src ./Libraries/Ethernet/lwip/src/core/pbuf.d ./Libraries/Ethernet/lwip/src/core/pbuf.o ./Libraries/Ethernet/lwip/src/core/pbuf.src ./Libraries/Ethernet/lwip/src/core/raw.d ./Libraries/Ethernet/lwip/src/core/raw.o ./Libraries/Ethernet/lwip/src/core/raw.src ./Libraries/Ethernet/lwip/src/core/stats.d ./Libraries/Ethernet/lwip/src/core/stats.o ./Libraries/Ethernet/lwip/src/core/stats.src ./Libraries/Ethernet/lwip/src/core/sys.d ./Libraries/Ethernet/lwip/src/core/sys.o ./Libraries/Ethernet/lwip/src/core/sys.src ./Libraries/Ethernet/lwip/src/core/tcp.d ./Libraries/Ethernet/lwip/src/core/tcp.o ./Libraries/Ethernet/lwip/src/core/tcp.src ./Libraries/Ethernet/lwip/src/core/tcp_in.d ./Libraries/Ethernet/lwip/src/core/tcp_in.o ./Libraries/Ethernet/lwip/src/core/tcp_in.src ./Libraries/Ethernet/lwip/src/core/tcp_out.d ./Libraries/Ethernet/lwip/src/core/tcp_out.o ./Libraries/Ethernet/lwip/src/core/tcp_out.src ./Libraries/Ethernet/lwip/src/core/timeouts.d ./Libraries/Ethernet/lwip/src/core/timeouts.o ./Libraries/Ethernet/lwip/src/core/timeouts.src ./Libraries/Ethernet/lwip/src/core/udp.d ./Libraries/Ethernet/lwip/src/core/udp.o ./Libraries/Ethernet/lwip/src/core/udp.src

.PHONY: clean-Libraries-2f-Ethernet-2f-lwip-2f-src-2f-core

