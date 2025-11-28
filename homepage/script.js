document.addEventListener("DOMContentLoaded", function() {

    // ---------- Surprise Me Button ----------
    const surpriseBtn = document.querySelector('.surprise-btn');
    const surpriseText = document.getElementById('surprise-text');

    if (surpriseBtn && surpriseText) {
        const facts = [
            "I have traveled to over 15 countries!",
            "I can speak three languages fluently.",
            "I'm a climbing enthusiast.",
            "I am OSHA certified."
        ];

        surpriseBtn.addEventListener('click', function () {
            const randomIndex = Math.floor(Math.random() * facts.length);
            surpriseText.textContent = facts[randomIndex];
        });
    }

    // ---------- Contact Form ----------
    const contactForm = document.getElementById('contact-form');
    const contactFeedback = document.getElementById('contact-feedback');

    if (contactForm && contactFeedback) {
        contactForm.addEventListener('submit', function(event) {
            event.preventDefault();

            const nameValue = document.getElementById('name').value.trim();

            contactFeedback.textContent =
                `Thank you, ${nameValue || "friend"}! Your message has been received.`;

            contactForm.reset();
        });
    }

});
