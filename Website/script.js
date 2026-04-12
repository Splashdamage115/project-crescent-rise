// Replace this value once the final trailer URL is available.
const HERO_VIDEO_URL = "";

const frame = document.querySelector("[data-video-frame]");
const placeholder = document.querySelector("[data-video-placeholder]");

if (frame && placeholder && HERO_VIDEO_URL.trim().length > 0) {
  frame.src = HERO_VIDEO_URL;
  frame.style.display = "block";
  placeholder.style.display = "none";
}

const layers = Array.from(document.querySelectorAll(".layer"));
const revealElements = Array.from(document.querySelectorAll("[data-animate]"));

const updateParallax = () => {
  const y = window.scrollY;

  for (const layer of layers) {
    const speed = Number(layer.dataset.speed || 0);
    layer.style.transform = `translate3d(0, ${-y * speed}px, 0)`;
  }
};

window.addEventListener("scroll", updateParallax, { passive: true });
window.addEventListener("load", updateParallax);

if ("IntersectionObserver" in window) {
  const revealObserver = new IntersectionObserver(
    (entries, observer) => {
      for (const entry of entries) {
        if (entry.isIntersecting) {
          entry.target.classList.add("in-view");
          observer.unobserve(entry.target);
        }
      }
    },
    {
      threshold: 0.18,
      rootMargin: "0px 0px -40px 0px"
    }
  );

  for (const element of revealElements) {
    revealObserver.observe(element);
  }
} else {
  for (const element of revealElements) {
    element.classList.add("in-view");
  }
}
