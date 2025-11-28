SELECT COUNT(*) AS "Number of 10.0 Movies"
FROM movies
JOIN ratings ON movies.id = ratings.movie_id
WHERE rating = 10.0;
