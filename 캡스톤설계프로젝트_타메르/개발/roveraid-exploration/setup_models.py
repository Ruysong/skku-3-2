"""Download official local models; no cloud inference or API key."""
from pathlib import Path
from concurrent.futures import ThreadPoolExecutor
import urllib.request
import zipfile

ROOT = Path(__file__).resolve().parent
FILES = {
    'models/yolo26n-pose.pt': 'https://github.com/ultralytics/assets/releases/download/v8.4.0/yolo26n-pose.pt',
    'models/Qwen3VL-2B-Instruct-Q4_K_M.gguf': 'https://huggingface.co/Qwen/Qwen3-VL-2B-Instruct-GGUF/resolve/main/Qwen3VL-2B-Instruct-Q4_K_M.gguf',
    'models/mmproj-Qwen3VL-2B-Instruct-Q8_0.gguf': 'https://huggingface.co/Qwen/Qwen3-VL-2B-Instruct-GGUF/resolve/main/mmproj-Qwen3VL-2B-Instruct-Q8_0.gguf',
    'runtime/llama.zip': 'https://github.com/ggml-org/llama.cpp/releases/download/b12317/llama-b12317-bin-win-cpu-x64.zip',
    'runtime/llama-vulkan.zip': 'https://github.com/ggml-org/llama.cpp/releases/download/b12317/llama-b12317-bin-win-vulkan-x64.zip',
    'samples/pedestrians.avi': 'https://raw.githubusercontent.com/opencv/opencv/4.x/samples/data/vtest.avi',
}

def download(item):
    name, url = item
    target = ROOT / name
    target.parent.mkdir(parents=True, exist_ok=True)
    if target.exists():
        print('Already available:', name, flush=True)
        return
    tmp = target.with_suffix(target.suffix + '.part')
    print('Downloading:', name, flush=True)
    req = urllib.request.Request(url, headers={'User-Agent': 'RoverAid-demo-setup'})
    with urllib.request.urlopen(req, timeout=60) as response, tmp.open('wb') as stream:
        count = 0
        while block := response.read(4 * 1024 * 1024):
            stream.write(block)
            count += len(block)
            if count // (128 * 1024 * 1024) != (count - len(block)) // (128 * 1024 * 1024):
                print(name, round(count / 1024**2), 'MiB', flush=True)
    tmp.replace(target)
    print('Ready:', name, target.stat().st_size, flush=True)

if __name__ == '__main__':
    with ThreadPoolExecutor(max_workers=3) as pool:
        list(pool.map(download, FILES.items()))
    with zipfile.ZipFile(ROOT / 'runtime/llama.zip') as z:
        z.extractall(ROOT / 'runtime/llama')
    with zipfile.ZipFile(ROOT / 'runtime/llama-vulkan.zip') as z:
        z.extractall(ROOT / 'runtime/llama-vulkan')
    print('Model setup complete.', flush=True)
