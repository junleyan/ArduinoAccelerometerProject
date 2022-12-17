import os
import csv


header = ['interval', 'hour', 'minute', 'second', 
        'acX', 'acY', 'acX', 
        'gyX', 'gyY', 'gyZ',
        'denoisedAcX', 'denoisedAcY', 'denoisedAcZ']


if __name__ == "__main__":
    
    txt_file = []
    for file in os.listdir(os.curdir):
        if file.endswith('TXT'):
            txt_file.append(file)

    for file in txt_file:
        with open(file) as r:
            content = r.readlines()
        for i in range(len(content)):
            tmp = content[i].replace('\n', '').split(',')
            tmp.append(int(tmp[4])-1884)
            tmp.append(int(tmp[5])+5760)
            tmp.append(int(tmp[6])-10620)
            content[i] = tmp

        with open(file.replace('.TXT', '.csv'), 'w', encoding='UTF8', newline='') as f:
            writer = csv.writer(f)
            writer.writerow(header)
            writer.writerows(content)