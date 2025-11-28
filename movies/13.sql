SELECT DISTINCT people.name FROM people
JOIN stars ON people.id = stars.person_id
WHERE stars.movie_id IN (
    SELECT s2.movie_id
    FROM stars s2
    JOIN people kb ON kb.id = s2.person_id
    WHERE kb.name = 'Kevin Bacon' AND kb.birth = 1958
)
AND people.id <>(
    SELECT id FROM people WHERE name = 'Kevin Bacon' AND birth = 1958
);
