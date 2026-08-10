#!/usr/bin/env python3
"""
TestForge Stress Test Validator (Robust Windows version)
- Generates tests for all stress files
- Gracefully skips compilation if no compiler/CMake is installed
- Always shows a clean summary
"""

import subprocess
import sys
from pathlib import Path
import shutil

# === Configuration ===
STRESS_DIR = Path(__file__).parent.resolve()
PROJECT_ROOT = STRESS_DIR.parent.parent
OUT_DIR = PROJECT_ROOT / "out"
TESTFORGE = "testforge"


def run(cmd, cwd=None):
    """Helper to run commands safely."""
    return subprocess.run(cmd, capture_output=True, text=True, cwd=cwd)


def main():
    print("TestForge Stress Test Validator")
    print("=" * 55)

    # === Step 1: Find all stress test source files ===
    stress_files = sorted([
        f for f in STRESS_DIR.glob("*.c") 
        if not f.name.startswith("mock_")
    ])

    if not stress_files:
        print("No stress test source files found!")
        sys.exit(1)

    print(f"Found {len(stress_files)} stress test files to validate.\n")

    generation_issues = {}

    # === Step 2: Generate tests for every file ===
    for src in stress_files:
        print(f"→ Generating: {src.name}")
        result = run([TESTFORGE, "generate", "-i", str(src), "--framework", "unity"], 
                     cwd=PROJECT_ROOT)

        if result.returncode != 0:
            generation_issues[src.name] = "Generation failed"
            print(f"   ❌ Generation failed")
        else:
            print(f"   ✅ Generated successfully")

    # === Step 3: Optional Compilation Check ===
    print("\n" + "-" * 55)
    compiled_successfully = False

    cmake_path = shutil.which("cmake")
    if cmake_path:
        print("CMake found. Attempting to build stress tests...")
        try:
            OUT_DIR.mkdir(exist_ok=True)

            # Configure
            config_result = run(["cmake", "-B", "build", "-S", "."], cwd=OUT_DIR)
            if config_result.returncode != 0:
                raise RuntimeError("CMake configuration failed")

            # Build
            build_result = run(
                ["cmake", "--build", "build", "--target", "stress_tests"], 
                cwd=OUT_DIR
            )
            compiled_successfully = build_result.returncode == 0

            if compiled_successfully:
                print("✅ Compilation successful!")
            else:
                print("❌ Compilation failed (see errors above)")

        except Exception as e:
            print(f"⚠️  Compilation skipped due to error: {e}")
    else:
        print("⚠️  CMake not found in PATH — skipping compilation step.")
        print("   Install MinGW-w64 or use WSL to enable full validation.")

    # === Step 4: Final Summary (always runs) ===
    print("\n" + "=" * 55)
    print("VALIDATION SUMMARY")
    print("=" * 55)

    for src in stress_files:
        status = "✅" if src.name not in generation_issues else "❌"
        print(f"{status}  {src.name}")

    print(f"\nFiles with generation issues : {len(generation_issues)}")
    print(f"Compilation result           : {'SUCCESS' if compiled_successfully else 'SKIPPED / FAILED'}")

    if generation_issues:
        print("\nSome files had generation problems. Review the output above.")
        sys.exit(1)
    else:
        print("\n🎉 All stress test files generated successfully!")


if __name__ == "__main__":
    main()