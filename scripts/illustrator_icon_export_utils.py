from win32com.client import GetActiveObject, CDispatch
from win32com.client.dynamic import Dispatch
from pythoncom import com_error

from tempfile import NamedTemporaryFile, TemporaryDirectory
from contextlib import contextmanager
from urllib.parse import urlsplit
from xml.dom import minidom
from pathlib import Path
import time

from selenium.webdriver import Chrome, ChromeOptions
import chromedriver_autoinstaller

from typing import ContextManager, Dict, Optional, Literal, Iterable


def generate_name_from_url(url: str) -> str:
    _, domain, path, *_ = urlsplit(url.lower())

    if domain == 'thenounproject.com' and path.startswith('/icon/'):
        return path[6:].rstrip('/').replace('-', '_')

    raise ValueError('The URL is not a valid Noun Project URL')

def generate_name_from_file(file: str | Path) -> str:
    if not isinstance(file, Path):
        file = Path(file)

    return file.stem.lower().replace('-', '_')

def get_com_object(clsid: str) -> CDispatch:
    try:
        return GetActiveObject(clsid)
    except com_error:
        return Dispatch(clsid)

@contextmanager
def create_temp_file() -> ContextManager[Path]:
    try:
        temp_file_obj = NamedTemporaryFile(
            mode='w',
            encoding='utf-8',
            suffix='.svg',
            delete=False
        )

        temp_file = Path(temp_file_obj.name)

        yield temp_file
    finally:
        temp_file_obj.close()
        temp_file.unlink()

def remove_author_from_noun_project_icons(raw_svg_xml: str) -> str:
    svg_tree = minidom.parseString(raw_svg_xml)

    for defs_tag in svg_tree.getElementsByTagName('defs'):
        #defs_tag.unlink()
        defs_tag.parentNode.removeChild(defs_tag)

    for use_tag in svg_tree.getElementsByTagName('use'):
        use_tag.parentNode.removeChild(use_tag)

    svg_xml = svg_tree \
        .toprettyxml(encoding=svg_tree.encoding) \
        .decode('utf-8')

    return svg_xml

def export_document_multiple_sizes(
    document: CDispatch,
    name: str,
    export_folder: Path | str,
    export_sizes: Dict[str, int | float]
) -> None:
    if not isinstance(export_folder, Path):
        export_folder = Path(export_folder)

    png_export_options = get_com_object(
        'Illustrator.ExportForScreensOptionsPNG24'
    )

    png_export_options.ScaleType = 1 # By width

    artboard = document.Artboards.Item(1)
    artboard_name = artboard.Name

    with TemporaryDirectory() as temp_dir:
        for size_name, size in export_sizes.items():
            png_export_options.ScaleTypeValue = size
            artboard.Name = f'{name}_{size_name}'

            document.ExportForScreens(
                ExportFolder=temp_dir,
                ExportFormat=7, # 32-bit depth PNG
                Options=png_export_options
            )

            temp_export_file = Path(temp_dir) \
                .joinpath(f'{size}w') \
                .joinpath(artboard.Name) \
                .with_suffix('.png')

            export_file = Path(export_folder) \
                .joinpath(temp_export_file.name)

            # Illustrator will throw an error if a file
            # with the same name already exists; it will
            # not overwrite it
            if export_file.exists():
                export_file.unlink()

            temp_export_file.rename(export_file)

    # Change artboard name back to original
    artboard.Name = artboard_name

def save_document(document: CDispatch, save_folder: str | Path) -> None:
    save_options = get_com_object(
        'Illustrator.IllustratorSaveOptions'
    )

    save_options.FlattenOutput = 1

    document.SaveAs(save_folder, save_options)

def create_document_with_transparency_grid(name: str, artboard_size: int | float) -> CDispatch:
    # AI document options
    document_presets = get_com_object(
        'Illustrator.DocumentPreset'
    )

    # The only way I could use scripting to turn on the
    # transparency grid was by selecting the video preset
    # The vidoe preset shows a bunch of unnecessary things
    # (e.g., cross hairs, safe area, etc.). This code
    # generates the artboard and then removes all of the
    # unnecessary stuff
    document_presets.DocumentTitle = name
    document_presets.DocumentTransparencyGrid = 3 # Dark
    document_presets.DocumentColorSpace = 2 #CMYK
    document_presets.DocumentUnits = 4 # Points
    document_presets.Height = artboard_size
    document_presets.Width = artboard_size

    document = illustrator.Documents.AddDocument(
        StartupPreset=illustrator.StartupPresetsList[1],
        PresetSettings=document_presets,
        ShowOptionsDialog=False
    )

    for i in range(1, document.Artboards.Count):
        document.Artboards.Remove(1)

    artboard = document.Artboards.Item(1)

    # Remove all of the grid helpers, like the video
    # ruler, cross hairs, safe areas, and center point
    illustrator.ExecuteMenuCommand('videoruler')
    artboard.ShowCrossHairs = False
    artboard.ShowSafeAreas = False
    artboard.ShowCenter = False

    return document

def color_all_path_items_white(document: CDispatch) -> None:
    # Turn every path item white
    for path_item in document.PathItems:
        path_item.FillColor.Cyan = 0
        path_item.FillColor.Magenta = 0
        path_item.FillColor.Yellow = 0
        path_item.FillColor.Black = 0

def get_icon_background(document: CDispatch, icon_type: str, icon_color: str) -> CDispatch:
    for layer in document.Layers:
        if layer.Name == icon_type:
            for group in layer.GroupItems:
                if group.Name == icon_color:
                    return group

    raise ValueError(
        f"No icon background with a type of '{icon_type}' and a color of '{icon_color}' exist"
    )

def return_or_open_document(document_file: str | Path) -> CDispatch:
    if not isinstance(document_file, Path):
        document_file = Path(document_file)

    for document in illustrator.Documents:
        if Path(document.FullName) == document_file:
            return document

    return illustrator.Open(document_file)

def download_noun_project_svg_icon(noun_project_url: str) -> str:
    with TemporaryDirectory() as temp_dir:
        options = ChromeOptions()

        options.add_argument(
            'user-data-dir=C:\\Users\\Riley\\AppData\\Local\\Google\\Chrome\\User Data\\Default'
        )
        #options.add_argument('window-size=1400,900')
        #options.add_argument('--headless')
        options.add_argument("--kiosk")

        options.add_experimental_option('prefs', {
            'download.default_directory': temp_dir
        })

        driver = Chrome(
            executable_path=chromedriver_autoinstaller.install(),
            options=options
        )

        driver.get(noun_project_url)

        driver.find_element(
            'xpath',
            '//button[text()="Get This Icon"]'
        ).click()

        driver.find_element(
            'xpath',
            '//label[@aria-label="Basic Download"]'
        ).click()

        driver.find_element(
            'xpath',
            '//button[contains(@class, "styles__ContinueButton")]'
        ).click()

        driver.find_element(
            'xpath',
            '//button[contains(normalize-space(concat(text()[1], text()[2])), "Download SVG")]'
        ).click()

        while True:
            temp_dir_files = list(Path(temp_dir).glob('*'))

            if len(temp_dir_files) > 0:
                svg_file = temp_dir_files[0]
                break

            time.sleep(1)

        driver.close()

        svg_xml = svg_file.read_text(encoding='utf-8')

    return svg_xml

icon_backgrounds_file = 'C:/Users/Riley/Documents/COVID-19/icon_backgrounds.ai'

def create_noun_project_sized_icons(
    path: str | Path,
    *,
    path_type: Literal['file', 'url']='file',
    name: Optional[str]=None,
    icon_type: Optional[str]='Horizontal',
    icon_color: Optional[str]='Red',
    icon_files_export_folder: str | Path,
    ai_file_export_folder: str | Path,
    svg_file_export_folder: Optional[str | Path]=None,
    artboard_size: Optional[int | float]=512,
    icon_sizes={
        'Bordered': 400,
        'Horizontal': 450,
        'Diagonal': 450
    },
    export_sizes: Optional[Dict[str, int | float]]={
        'vs': 24,
        's': 32,
        'n': 40,
        'l': 48,
        'vl': 56,
        '512x512': 512
    }
) -> None:
    if path_type == 'file':
        if name is None:
            name = generate_name_from_file(path)

        with open(path, 'r', encoding='utf-8') as f:
            raw_svg_xml = f.read()
    elif path_type == 'url':
        if name is None:
            name = generate_name_from_url(path)

        raw_svg_xml = download_noun_project_svg_icon(path)

        if svg_file_export_folder is not None:
            if not isinstance(svg_file_export_folder, Path):
                svg_file_export_folder = Path(svg_file_export_folder)

            svg_file_export_folder \
                .joinpath(name) \
                .with_suffix('.svg') \
                .write_text(raw_svg_xml, encoding='utf-8')
    else:
        raise ValueError("`path_type` must be equal to either 'file' or 'url'")

    # This goes into the XML and manually deletes the author
    # paths/defs. This is only applicable to Noun Project icons
    # because their SVGs are formatted in a way that makes the
    # author paths/defs identifiable
    svg_xml = remove_author_from_noun_project_icons(raw_svg_xml)

    with create_temp_file() as temp_file:
        temp_file.write_text(svg_xml, encoding='utf-8')

        document = create_document_with_transparency_grid(name, artboard_size)

        # Load the SVG into the newly created artboard
        document.GroupItems.CreateFromFile(temp_file)

    # It might be the case that some of the icons are
    # represented by strokes rather than fills, so expand
    # is used to convert them all to using fills
    document.SelectObjectsOnActiveArtboard()
    illustrator.ExecuteMenuCommand('Expand3')

    # Turn on transparency grid
    if not document.IsTransparencyGridVisible():
        illustrator.ExecuteMenuCommand('TransparencyGrid Menu Item')

    # Group all paths
    document.SelectObjectsOnActiveArtboard()
    illustrator.ExecuteMenuCommand('group')

    # Color all path items white
    color_all_path_items_white(document)

    icon_layer = document.Layers.Item(1)
    icon_layer.Name = 'Icon'

    # Copy the selected background from the `icon_backgrounds.ai`
    # file into the background layer
    icon_backgrounds = return_or_open_document(icon_backgrounds_file)

    # Find the name with the specified icon type and color by
    # iterating of the layers and then the groups.
    background_group = get_icon_background(
        document=icon_backgrounds,
        icon_type=icon_type,
        icon_color=icon_color
    )

    background_layer = document.Layers.Add()
    background_layer.MoveToEnd(document)
    background_layer.Name = 'Background'

    icon_backgrounds.Activate()
    background_group.Copy()

    document.Activate()
    background_layer.Paste()

    # For some reason, it wants to save even though nothing
    # changed. 'Saving=2' makes it close without asking.
    ###################icon_backgrounds.Close(Saving=2)

    # Resize the icon group based off of the larger of the two sides
    icon_group = icon_layer.GroupItems.Item(1)
    scale_factor = icon_sizes[icon_type] * 100 / max(
        icon_group.Width,
        icon_group.Height
    )

    icon_group.Resize(scale_factor, scale_factor)

    icon_group.Left = (artboard_size - icon_group.Width) / 2
    icon_group.Top = (artboard_size + icon_group.Height) / 2

    # Set artboard name to supplied name
    artboard = document.Artboards.Item(1)
    artboard.Name = name

    """
    return document
    export_document_multiple_sizes(
        document=document,
        export_folder=icon_files_export_folder,
        export_sizes=export_sizes
    )
    """

    save_document(
        document=document,
        save_folder=ai_file_export_folder
    )

    return document

def combine_ai_files(
    files: Iterable[Path | str],
    save_file: Path | str,
    document_name: str='combined',
    artboard_size: int | float=512,
    spacing: int | float=20
) -> None:
    combined_document = create_document_with_transparency_grid(document_name, artboard_size)

    artboard = combined_document.Artboards.Item(1)

    for i, file in enumerate(files):
        if i > 0:
            artboard = combined_document.Artboards.Add((
                (artboard_size * i) + (spacing * i),
                artboard_size,
                artboard_size * (i + 1) + (spacing * i),
                0
            ))

        if isinstance(file, Path):
            name = file.stem
        else:
            name = Path(file).ste,

        artboard.Name = name

        group = combined_document.GroupItems.CreateFromFile(file)

        group.Translate(
            (artboard_size * i) + (spacing * i),
            0
        )

        group.Name = name

    document.RearrangeArtboards(
        ArtboardLayout=2,
        ArtboardRowsOrCols=3,
        ArtboardSpacing=spacing
    )

    document.SaveAs(save_file)
