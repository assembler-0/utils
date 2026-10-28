/* SPDX-License-Identifier: GPL-2.0-only */
/**
 * @file cpuvendor.h
 * @brief CPUID vendor strings for Physical CPUs (Leaf 0x0) and Hypervisors (Leaf 0x40000000)
 * @copyright (c) 2026 assembler-0
 */

#ifndef CPUVENDOR_H
#define CPUVENDOR_H

#ifndef CPUVENDOR_FREESTANDING
#include <stdint.h>
#include <stdbool.h>
#include <cpuid.h>
#endif /* !CPUVENDOR_FREESTANDING */

#define CPUVENDOR_ID_INTEL              "GenuineIntel"
#define CPUVENDOR_ID_INTEL_RARE         "GenuineIotel"  /* Haswell Xeon E3 bitflip */
#define CPUVENDOR_ID_AMD                "AuthenticAMD"
#define CPUVENDOR_ID_AMD_OLD            "AMDisbetter!"  /* K5 Engineering Sample */
#define CPUVENDOR_ID_AMD_OLD_ALT        "AMD ISBETTER"
#define CPUVENDOR_ID_VIA                "VIA VIA VIA "
#define CPUVENDOR_ID_CENTAUR            "CentaurHauls"
#define CPUVENDOR_ID_CYRIX              "CyrixInstead"
#define CPUVENDOR_ID_TRANSMETA          "GenuineTMx86"
#define CPUVENDOR_ID_TRANSMETA_OLD      "TransmetaCPU"  /* Used in Leaf 0x80000000 */
#define CPUVENDOR_ID_NEXGEN             "NexGenDriven"
#define CPUVENDOR_ID_UMC                "UMC UMC UMC "
#define CPUVENDOR_ID_SIS                "SiS SiS SiS "
#define CPUVENDOR_ID_NSC                "Geode by NSC"
#define CPUVENDOR_ID_RISE               "RiseRiseRise"
#define CPUVENDOR_ID_VORTEX             "Vortex86 SoC"
#define CPUVENDOR_ID_ZHAOXIN            "  Shanghai  " 
#define CPUVENDOR_ID_HYGON              "HygonGenuine"
#define CPUVENDOR_ID_RDC                "Genuine  RDC"
#define CPUVENDOR_ID_TI                 "TI CPU@Work "

/* Soft Cores / FPGA */
#define CPUVENDOR_ID_AO486              "MiSTer AO486"
#define CPUVENDOR_ID_AO486_OLD          "GenuineAO486"

/* Binary Translators & Emulators reporting via Leaf 0 */
#define CPUVENDOR_ID_ELBRUS             "E2K MACHINE "
#define CPUVENDOR_ID_CONNECTIX          "ConnectixCPU"
#define CPUVENDOR_ID_MS_VPC7            "Virtual CPU "
#define CPUVENDOR_ID_INSIGNIA           "Insignia 586"
#define CPUVENDOR_ID_COMPAQ_FX32        "Compaq FX!32"
#define CPUVENDOR_ID_POWERVM_LX86       "PowerVM Lx86"
#define CPUVENDOR_ID_NEKO_PROJECT       "Neko Project"

/* Hypervisors (CPUID Leaf 0x40000000: EBX, ECX, EDX)                        */
#define CPUVENDOR_HV_ID_QEMU_TCG               "TCGTCGTCGTCG"
#define CPUVENDOR_HV_ID_KVM                    "KVMKVMKVM\0\0\0"
#define CPUVENDOR_HV_ID_KVM_HYPERV             "Linux KVM Hv"    /* KVM with Hyper-V enlightenments */
#define CPUVENDOR_HV_ID_VMWARE                 "VMwareVMware"
#define CPUVENDOR_HV_ID_VIRTUALBOX             "VBoxVBoxVBox"
#define CPUVENDOR_HV_ID_XEN                    "XenVMMXenVMM"
#define CPUVENDOR_HV_ID_HYPERV                 "Microsoft Hv"
#define CPUVENDOR_HV_ID_PARALLELS              " prl hyperv "
#define CPUVENDOR_HV_ID_PARALLELS_ALT          " lrpepyh vr "    /* Endianness reversed */
#define CPUVENDOR_HV_ID_BHYVE                  "bhyve bhyve "
#define CPUVENDOR_HV_ID_BHYVE_OLD              "BHyVE BHyVE "
#define CPUVENDOR_HV_ID_QNX                    "QNXQVMBSQG\0\0" 
#define CPUVENDOR_HV_ID_ACRN                   "ACRNACRNACRN"
#define CPUVENDOR_HV_ID_APPLE_VZ               "Apple VZ\0\0\0\0"
#define CPUVENDOR_HV_ID_APPLE_ROSETTA2         "VirtualApple"
#define CPUVENDOR_HV_ID_MS_XTA                 "MicrosoftXTA"
#define CPUVENDOR_HV_ID_INTEL_HAXM             "HAXMHAXMHAXM"
#define CPUVENDOR_HV_ID_INTEL_KGT              "EVMMEVMMEVMM"
#define CPUVENDOR_HV_ID_NETBSD_NVMM            "___ NVMM ___"
#define CPUVENDOR_HV_ID_OPENBSD_VMM            "OpenBSDVMM58"
#define CPUVENDOR_HV_ID_JAILHOUSE              "Jailhouse\0\0\0"
#define CPUVENDOR_HV_ID_LOCKHEED_SRE           "SRESRESRESRE"

typedef char cpuvendor_id[13];

#ifndef CPUVENDOR_FREESTANDING

/**
 * @brief Get cpuid vendor
 * @param __out_vendor[in,out] Name buffer
 */
static inline void cpuvendor_get_vendor(cpuvendor_id __out_vendor) {
	uint32_t eax, ebx, ecx, edx;
	__get_cpuid(0, &eax, &ebx, &ecx, &edx);
	*(uint32_t *)(__out_vendor + 0) = ebx;
    *(uint32_t *)(__out_vendor + 4) = edx;
    *(uint32_t *)(__out_vendor + 8) = ecx;
    __out_vendor[12] = '\0';
}

/**
 * @brief Get hypervisor cpuid vendor (if available)
 * @param __out_hv_vendor[in,out] Name buffer
 * @return 'false' on unsupported
 */
static inline bool cpuvendor_get_hypervisor(cpuvendor_id __out_hv_vendor) {
    uint32_t eax, ebx, ecx, edx;
    __get_cpuid(1, &eax, &ebx, &ecx, &edx);
    if (!(ecx & (1u << 31))) {
        return false;
    }
    __get_cpuid(0x40000000, &eax, &ebx, &ecx, &edx);
    *(uint32_t *)(__out_hv_vendor + 0) = ebx;
    *(uint32_t *)(__out_hv_vendor + 4) = ecx;
    *(uint32_t *)(__out_hv_vendor + 8) = edx;
    __out_hv_vendor[12] = '\0';
    return true;
}

#endif /* !CPUVENDOR_FREESTANDING */

#endif /* CPUVENDOR_H */
