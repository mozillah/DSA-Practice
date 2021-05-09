import os
import cv2
from matplotlib import pyplot as plt
import glob
import numpy as np
import xml.etree.ElementTree as ET
from lxml import etree
OUTPUT_DIR ="output"
INPUT_DIR = "input"


def drawRoBBs(frame=None,frame_path=None, ann_path=None):
    '''Draws rotated bounding boxes to the corresponding image and r
    urns the annotated image
    This function can be called in two separate ways. 
    
    Args:
    fr: RGB Image in uint8 format
    ann_dir: Path of the annotation file
    
    returns:
    anoted RGB image in uint8 format  
    '''
    frame_out = frame.copy()
    annotation_paths = get_annotation_paths(frame_path, annotation_formats)
    abs_path = os.path.abspath(frame_path)
    folder_name = os.path.dirname(frame_path)
    image_name = os.path.basename(frame_path)

    img_height, img_width, depth = (str(number) for number in frame.shape)

    if os.path.exists(ann_path):
        objects = np.loadtxt(ann_path, dtype=str)
        for ann_path_ in annotation_paths:
                    if not os.path.isfile(ann_path_):
                        create_PASCAL_VOC_xml(ann_path_, abs_path, folder_name, image_name, img_height, img_width, depth)

        for obj in objects:
            # Get box properties
            x = float(obj[1])
            y = float(obj[2])
            w = float(obj[3])
            h = float(obj[4])
            angle = float(obj[5])*np.pi/180
            c, s = np.cos(angle), np.sin(angle)
            R = np.asarray([[-c, -s], [s, -c]])
            pts = np.asarray([[-w / 2, -h / 2], [w / 2, -h / 2], [w / 2, h / 2], [-w / 2, h / 2]])
            print(pts)
            rot_pts = []
            for pt in pts:
                rot_pts.append(([x, y] + pt @ R).astype(int))
            contours = np.array([rot_pts[0], rot_pts[1], rot_pts[2], rot_pts[3]])
            print(contours)
            xx = contours[0][0]
            yy = contours[0][1]
            ww = contours[2][0] 
            hh = contours[2][1] 
            save_bounding_box(annotation_paths, 0, (xx, yy),(ww, hh), img_width, img_height)
            # cv2.polylines(frame_out, pts=[contours], color=(0, 0, 255), isClosed=True, thickness=3)
    return frame_out


def write_xml(xml_str, xml_path):
    # remove blank text before prettifying the xml
    parser = etree.XMLParser(remove_blank_text=True)
    root = etree.fromstring(xml_str, parser)
    # prettify
    xml_str = etree.tostring(root, pretty_print=True)
    # save to file
    with open(xml_path, 'wb') as temp_xml:
        temp_xml.write(xml_str)


def create_PASCAL_VOC_xml(xml_path, abs_path, folder_name, image_name, img_height, img_width, depth):
    # By: Jatin Kumar Mandav
    annotation = ET.Element('annotation')
    ET.SubElement(annotation, 'folder').text = folder_name
    ET.SubElement(annotation, 'filename').text = image_name
    ET.SubElement(annotation, 'path').text = abs_path
    source = ET.SubElement(annotation, 'source')
    ET.SubElement(source, 'database').text = 'Unknown'
    size = ET.SubElement(annotation, 'size')
    ET.SubElement(size, 'width').text = img_width
    ET.SubElement(size, 'height').text = img_height
    ET.SubElement(size, 'depth').text = depth
    ET.SubElement(annotation, 'segmented').text = '0'

    xml_str = ET.tostring(annotation)
    write_xml(xml_str, xml_path)

def voc_format(class_name, point_1, point_2):
    # Order: class_name xmin ymin xmax ymax
    xmin, ymin = min(point_1[0], point_2[0]), min(point_1[1], point_2[1])
    xmax, ymax = max(point_1[0], point_2[0]), max(point_1[1], point_2[1])
    items = map(str, [class_name, xmin, ymin, xmax, ymax])
    return items
    
def append_bb(ann_path, line, extension):
    if '.txt' in extension:
        with open(ann_path, 'a') as myfile:
            myfile.write(line + '\n') # append line
    if '.xml' in extension:

        class_name, xmin, ymin, xmax, ymax = line

        tree = ET.parse(ann_path)
        annotation = tree.getroot()

        obj = ET.SubElement(annotation, 'object')
        ET.SubElement(obj, 'name').text = class_name
        ET.SubElement(obj, 'pose').text = 'Unspecified'
        ET.SubElement(obj, 'truncated').text = '0'
        ET.SubElement(obj, 'difficult').text = '0'

        bbox = ET.SubElement(obj, 'bndbox')
        ET.SubElement(bbox, 'xmin').text = xmin
        ET.SubElement(bbox, 'ymin').text = ymin
        ET.SubElement(bbox, 'xmax').text = xmax
        ET.SubElement(bbox, 'ymax').text = ymax

        xml_str = ET.tostring(annotation)
        write_xml(xml_str, ann_path)


def get_annotation_paths(img_path, annotation_formats):
    annotation_paths = []
    for ann_dir, ann_ext in annotation_formats.items():
        new_path = os.path.join(OUTPUT_DIR, ann_dir)
        new_path = img_path.replace(INPUT_DIR, new_path, 1)
        pre_path, img_ext = os.path.splitext(new_path)
        new_path = new_path.replace(img_ext, ann_ext, 1)
        annotation_paths.append(new_path)
    return annotation_paths

def save_bounding_box(annotation_paths, class_index, point_1, point_2, width, height):
    for ann_path in annotation_paths:
        # if '.txt' in ann_path:
        #     line = yolo_format("person", point_1, point_2, width, height)
        #     append_bb(ann_path, line, '.txt')
        print(ann_path)
        if '.xml' in ann_path:
            line = voc_format("person", point_1, point_2)
            append_bb(ann_path, line, '.xml')

annotation_formats = {'PASCAL_VOC' : '.xml', 'YOLO_darknet' : '.txt'}
annotation_formats = {'PASCAL_VOC' : '.xml'}

vid_folder = './Meeting'

files=glob.glob(os.path.join(vid_folder, "*.jpg"))

for img in files:
    ann_path = img[:-4] +".txt"
    fr = cv2.cvtColor(cv2.imread(img), cv2.COLOR_BGR2RGB)
    fr_bb = drawRoBBs(frame=fr,frame_path=img, ann_path=ann_path)
# fr_id = '000120'



