const fs = require("fs");
const path = require("path");

function parseArgs(argv) {
  const args = {};
  for (let i = 2; i < argv.length; i += 1) {
    const key = argv[i];
    if (!key.startsWith("--")) continue;
    const name = key.slice(2);
    const value = argv[i + 1] && !argv[i + 1].startsWith("--") ? argv[++i] : "true";
    args[name] = value;
  }
  return args;
}

function requireTesseract(moduleHint) {
  const candidates = [
    moduleHint,
    process.env.TESSERACTJS_MODULE,
    "tesseract.js",
    "C:/Users/rruys/.cache/codex-runtimes/codex-primary-runtime/dependencies/node/node_modules/tesseract.js",
  ].filter(Boolean);

  const errors = [];
  for (const candidate of candidates) {
    try {
      return require(candidate);
    } catch (error) {
      errors.push(`${candidate}: ${error.message}`);
    }
  }

  throw new Error(`Cannot load tesseract.js.\n${errors.join("\n")}`);
}

function parseBool(value) {
  return String(value || "").toLowerCase() === "true";
}

function requiredLangs(lang) {
  return lang
    .split("+")
    .map((item) => item.trim())
    .filter(Boolean);
}

function hasLangData(dir, lang) {
  if (!dir || !fs.existsSync(dir)) return false;
  const names = [
    `${lang}.traineddata`,
    `${lang}.traineddata.gz`,
    `${lang}.traineddata.cache`,
  ];
  return names.some((name) => fs.existsSync(path.join(dir, name)));
}

function assertLocalLangData(lang, langPath, cachePath, allowDownload) {
  if (allowDownload) return;

  const missing = requiredLangs(lang).filter(
    (item) => !hasLangData(langPath, item) && !hasLangData(cachePath, item)
  );

  if (missing.length > 0) {
    throw new Error(
      [
        `Missing local Tesseract language data: ${missing.join(", ")}`,
        `Put *.traineddata or *.traineddata.gz in: ${langPath}`,
        "Or rerun new-review.ps1 with -AllowDownload if network access is intentional.",
      ].join("\n")
    );
  }
}

async function main() {
  const args = parseArgs(process.argv);
  const imagesDir = path.resolve(args.imagesDir || "");
  const outDir = path.resolve(args.outDir || "");
  const lang = args.lang || "eng";
  const cachePath = path.resolve(args.cachePath || path.join(outDir, ".tess-cache"));
  const langPath = path.resolve(args.langPath || cachePath);
  const allowDownload = parseBool(args.allowDownload);
  const moduleHint = args.tesseractModule;

  if (!fs.existsSync(imagesDir)) throw new Error(`Missing imagesDir: ${imagesDir}`);
  fs.mkdirSync(outDir, { recursive: true });
  fs.mkdirSync(cachePath, { recursive: true });
  fs.mkdirSync(langPath, { recursive: true });
  assertLocalLangData(lang, langPath, cachePath, allowDownload);

  const images = fs
    .readdirSync(imagesDir)
    .filter((name) => /\.png$/i.test(name))
    .sort((a, b) => a.localeCompare(b, undefined, { numeric: true }))
    .map((name) => path.join(imagesDir, name));

  if (images.length === 0) throw new Error(`No PNG pages found in ${imagesDir}`);

  const { createWorker } = requireTesseract(moduleHint);
  const worker = await createWorker(lang, 1, {
    cachePath,
    langPath,
    logger: (message) => {
      if (message.status && typeof message.progress === "number") {
        const progress = Math.round(message.progress * 100);
        process.stderr.write(`\r${message.status} ${progress}%`);
      }
    },
  });

  const pages = [];
  for (let index = 0; index < images.length; index += 1) {
    const image = images[index];
    const pageNumber = index + 1;
    process.stderr.write(`\nOCR page ${pageNumber}/${images.length}: ${path.basename(image)}\n`);
    const result = await worker.recognize(image);
    const text = (result.data.text || "").replace(/\r\n/g, "\n").trim();
    const confidence = Math.round(result.data.confidence || 0);
    const page = {
      page: pageNumber,
      image: path.relative(outDir, image).replace(/\\/g, "/"),
      confidence,
      text,
    };
    pages.push(page);
    fs.writeFileSync(path.join(outDir, `page-${String(pageNumber).padStart(3, "0")}.txt`), `${text}\n`, "utf8");
    fs.writeFileSync(path.join(outDir, `page-${String(pageNumber).padStart(3, "0")}.json`), JSON.stringify(page, null, 2), "utf8");
  }

  await worker.terminate();
  fs.writeFileSync(path.join(outDir, "ocr-pages.json"), JSON.stringify({ lang, pages }, null, 2), "utf8");
  process.stderr.write("\nDone.\n");
}

main().catch((error) => {
  console.error(error.stack || error.message);
  process.exit(1);
});
