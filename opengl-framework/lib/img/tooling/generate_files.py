# ---HOW TO---
# Create one function per file you want to generate. The file will have the same name as the function.
# Each function should compute and return the content of the corresponding file as a string.
# Then call `generate`:
# ```python
# def some_function_that_will_generate_a_file():
#     return "The content of the file"
#
# def another_function():
#     return "Another file"
#
# generate(
#     folder="generated", # The name of the output folder where the files will be generated. /!\ Don't put anything manually in that folder, it is erased and regenerated each time you call `generate()`.
#     files=[
#         some_function_that_will_generate_a_file,
#         another_function,
#     ]
# )
# ```
# ------------

def clear_generated_folder(path_relative_to_project_root):
    import shutil
    import os
    folder = output_folder(path_relative_to_project_root)
    if (os.path.isdir(folder)):
        shutil.rmtree(folder)
    os.makedirs(folder)


def output_folder(path_relative_to_project_root):
    import os
    import sys
    from pathlib import Path
    root_folder = Path(sys.argv[0]).parent
    return os.path.join(root_folder, path_relative_to_project_root)


def generate_one(function, path_relative_to_project_root, calling_file):
    generate_file(function.__name__, function(),
                  path_relative_to_project_root, calling_file)


def generate_file(name, content, path_relative_to_project_root, calling_file):
    import os
    with open(os.path.join(output_folder(path_relative_to_project_root), name) + ".inl", 'w') as f:
        f.write(heading(name, calling_file) + content)


def heading(function_name, calling_file):
    import os
    import sys
    path_to_generator = os.path.basename(
        calling_file if calling_file else sys.argv[0])
    return f"""/* -----------------------------------------------------------------------------
 * This file was automatically generated by a Python script.
 * PLEASE DON'T EDIT IT DIRECTLY, your changes would be overwritten the next time the script is run.
 * Instead, go to "{path_to_generator}" and edit the "{function_name}" function there.
 * -----------------------------------------------------------------------------
 */
"""


def generate(folder="generated", files=[], should_apply_clang_format=True, calling_file=""):
    clear_generated_folder(folder)
    for function in files:
        generate_one(function, folder, calling_file)

    if should_apply_clang_format:
        from os import path
        from pathlib import Path
        from importlib.machinery import SourceFileLoader
        apply_clang_format = SourceFileLoader(
            "apply_clang_format",
            path.join(Path(path.abspath(__file__)).parent, "apply_clang_format.py")).load_module()
        apply_clang_format.apply_clang_format(
            folder=output_folder(folder),
            print_result=False,
        )